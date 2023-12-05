//
//
// Created by Max Norfolk on 10/15/23.


#pragma once

#include <string>
#include <utility>
#include <vector>
#include <experimental/filesystem>


#include "core/fishstore.h"
#include "device/null_disk.h"
#include "adapters/simdjson_adapter.h"

#include "safe_typedef.h"

#define SCALE_FACTOR 1
#define MAX_RECORDS 10000

typedef fishstore::device::NullDisk disk_t;
typedef fishstore::adapter::SIMDJsonAdapter adapter_t;
using store_t = fishstore::core::FishStore<disk_t, adapter_t>;


typedef fishstore::adapter::StringRef StringRef;

typedef uint64_t PsfAddress;
typedef uint32_t PsfId;

// maps fishstore Record => nullable int
typedef std::function<fishstore::adapter::NullableInt(StringRef)> PsfFallback;

typedef std::function<bool(StringRef)> FilterFunction;

// Creates a fallback Psf function that can be called on a record and initializer an adapter with the correct
// fields necessary for the PSF to work properly
template<typename A>
PsfFallback makeFallback(fishstore::ezpsf::PsfInfo ez_psf, typename A::parser_t *&parser) {
    parser = A::NewParser(ez_psf.fields);
    return [ez_psf, parser](StringRef payload) {
        parser->Load(payload.Data(), payload.Length());
        printf("Payload: [%.*s]\n", (int) payload.Length(), payload.Data());
        bool check = parser->HasNext();
        assert(check);

        // get fields &
        auto record = parser->NextRecord().GetFields();
        if (record.size() != ez_psf.fields.size())
            return fishstore::adapter::NullableInt{false};

        int res = 0;
        bool has_value = ez_psf.psf(&record, &res);
        return fishstore::adapter::NullableInt{!has_value, res};
    };
}

constexpr PsfId FS_ID = -1;

struct PsfInfo {
    PsfInfo(PsfId id, PsfFallback fallback) : id(id), fallback(std::move(fallback)) {}

    PsfInfo(PsfId id, PsfAddress start, PsfAddress end, PsfFallback fallback)
            : id(id), start(start), end(end), preference(0), fallback(std::move(fallback)) {}

    PsfInfo(PsfId id, PsfAddress start, PsfAddress end, int selectivity) : id(id), start(start), end(end),
                                                                           preference(selectivity) {}

    PsfId id;
    PsfAddress start;
    PsfAddress end;
    int preference; // prefer higher preference for psfs scans
    PsfFallback fallback;
};

// Maps PSF id to PsfInfo
typedef std::map<PsfId, PsfInfo> PsfMap;

struct PsfAction {
    PsfAction(std::string ez_psf, uint64_t records_before, uint64_t records_covered)
            : ez_psf(std::move(ez_psf)),
              records_before(records_before),
              records_covered(records_covered) {}

    std::string ez_psf;
    uint64_t records_before;
    uint64_t records_covered;
};

// map argument will map a number of records to insert, before registering the PSF by calling the associated function
std::pair<std::unique_ptr<store_t>, PsfMap>
storeFromFile(const std::string &file_path, const std::vector<PsfAction> &actions) {
    static int counter = 0;
    const std::string fishstore_file_name = "fishstore_out_" + std::to_string(counter++);
    const size_t store_size = 1UL << 31;
    const size_t table_size = 1UL << 24;


    std::vector<std::string> batches;
    std::ifstream in(file_path);

    assert(!in.fail());

    std::string line;
    size_t json_batch_count, line_count, record_count;
    json_batch_count = line_count = record_count = 0;

    size_t json_batch_size = 1;

    while (std::getline(in, line)) {

        if (json_batch_count == 0 || line_count == json_batch_size) {
            line_count = 1;
            ++json_batch_count;
            batches.push_back(line);
        } else {
            batches.back() += line;
            json_batch_count++;
        }
        ++record_count;
        if (batches.size() >= MAX_RECORDS)
            break;
    }

    auto batches_copy(batches);
    for (int i = 1; i < SCALE_FACTOR; ++i) {
        batches.insert(batches.end(), batches_copy.begin(), batches_copy.end());
    }

    printf("everything ingested (%ld batches)\n", batches.size());

    std::experimental::filesystem::create_directory(fishstore_file_name);
    auto store = std::make_unique<store_t>(table_size, store_size, fishstore_file_name);
    store->StartSession();

    enum class ActionType {
        REG,
        DEREG
    };

    PsfMap return_map;
    // maps timestamp to an action => register or deregister PSF with id
    std::map<PsfAddress, std::pair<ActionType, PsfId>> psf_map;
    for (const auto &item: actions) {
        auto psf_lookup = store->MakeEzPsf(item.ez_psf);
        psf_map.insert({item.records_before, {ActionType::REG, psf_lookup.id}});
        psf_map.insert({item.records_before + item.records_covered,
                        {ActionType::DEREG, psf_lookup.id}});

        // TODO don't throw away parsers => not memory safe
        adapter_t::parser_t *parser;
        auto fallback = makeFallback<adapter_t>(store->getPsfInfoById(psf_lookup.id), parser);

        PsfInfo info(psf_lookup.id, fallback);
        return_map.emplace(psf_lookup.id, info);
    }


    uint64_t i = 0;
    for (auto &it: psf_map) {
        uint64_t needed_i = it.first;
        for (; i < needed_i; ++i) {
            store->BatchInsert(batches[i], 1);
        }
        store->Refresh();


        // Register PSF
        std::vector<ParserAction> parser_actions;
        PsfId psf_id = it.second.second;
        switch (it.second.first) { // psf_map => action type
            case ActionType::REG:
                PsfAddress start_addr;
                parser_actions.push_back({REGISTER_INLINE_PSF, psf_id});
                store->ApplyParserShift(parser_actions, [&start_addr](PsfAddress s_addr) {
                    start_addr = s_addr;
                });
                return_map.at(psf_id).start = start_addr;
                store->CompleteAction(true);
                printf("finished inserting %8lu/%lu records before registering.   psf %d [Address:%lu].\n",
                       i, batches.size(), psf_id, start_addr);
                break;
            case ActionType::DEREG:

                parser_actions.push_back({DEREGISTER_INLINE_PSF, psf_id});
                PsfAddress end_addr = store->ApplyParserShift(parser_actions, [](PsfAddress s_addr) {});
                return_map.at(psf_id).end = end_addr;
                store->CompleteAction(true);
                printf("finished inserting %8lu/%lu records before deregistering. psf %d  [Address:%lu].\n",
                       i, batches.size(), psf_id, end_addr);
                break;
        }
    }
    printf("Finished Main ingestions: [Remaining]\n");
    // insert everything remaining
    for (; i < batches.size(); ++i)
        store->BatchInsert(batches[i], 1);
    store->Refresh();


    return {std::move(store), return_map};
}