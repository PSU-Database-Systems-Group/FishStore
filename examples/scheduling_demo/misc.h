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
#define MAX_RECORDS 1000000

typedef fishstore::device::NullDisk disk_t;
typedef fishstore::adapter::SIMDJsonAdapter adapter_t;
using store_t = fishstore::core::FishStore<disk_t, adapter_t>;


typedef fishstore::adapter::StringRef StringRef;


/*struct PsfTimestamp : Value<uint64_t, PsfTimestamp> {
    PsfTimestamp(uint64_t t) : Value<uint64_t, PsfTimestamp>(t) {}
};*/

typedef uint64_t PsfTimestamp;

// maps fishstore Record => nullable int
typedef std::function<fishstore::adapter::NullableInt(StringRef)> PsfFallback;

// Creates a fallback Psf function that can be called on a record and initializer an adapter with the correct
// fields necessary for the PSF to work properly
template<typename A>
PsfFallback makeFallback(fishstore::ezpsf::PsfInfo ez_psf, typename A::parser_t *&parser) {
    parser = A::NewParser(ez_psf.fields);
    return [ez_psf, parser](StringRef payload) {
        parser->Load(payload.Data(), payload.Length());
        assert(parser->HasNext());

        auto record = parser->NextRecord();
        int res;
        bool has_value = ez_psf.psf(&record, &res);
        return fishstore::adapter::NullableInt{!has_value, res};
    };
}

struct PsfInfo {
    PsfInfo(uint32_t id, PsfFallback fallback) : id(id), fallback(std::move(fallback)) {}

    PsfInfo(uint32_t id, PsfTimestamp start, PsfTimestamp end) : id(id), start(start), end(end), preference(0) {}

    PsfInfo(uint32_t id, PsfTimestamp start, PsfTimestamp end, int selectivity) : id(id), start(start), end(end),
                                                                                  preference(selectivity) {}

    uint32_t id;
    PsfTimestamp start;
    PsfTimestamp end;
    int preference; // prefer higher preference for psfs scans
    PsfFallback fallback;
};

// Maps PSF id to PsfInfo
typedef std::map<uint32_t, PsfInfo> PsfMap;

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
    std::map<uint64_t, std::pair<ActionType, uint32_t>> psf_map;
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
        std::vector<ParserAction> parser_actions;
        uint32_t psf_id = it.second.second;
        switch (it.second.first) { // psf_map => action type
            case ActionType::REG:
                printf("finished inserting %8lu/%lu records before registering.   psf %d.\n", i, batches.size(),
                       psf_id);
                parser_actions.push_back({REGISTER_INLINE_PSF, psf_id});
                store->ApplyParserShift(parser_actions, [psf_id, &return_map](uint64_t s_addr) {
                    return_map.at(psf_id).start = s_addr;
                });
                store->CompleteAction(true);
                break;
            case ActionType::DEREG:
                printf("finished inserting %8lu/%lu records before deregistering. psf %d.\n", i, batches.size(),
                       psf_id);
                parser_actions.push_back({DEREGISTER_INLINE_PSF, psf_id});
                uint64_t e_addr = store->ApplyParserShift(parser_actions, [](uint64_t s_addr) {});
                return_map.at(psf_id).end = e_addr;
                store->CompleteAction(true);
                break;
        }
    }

    // insert everything remaining
    for (; i < batches.size(); ++i)
        store->BatchInsert(batches[i], 1);
    store->Refresh();


    return {std::move(store), return_map};
}