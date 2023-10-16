//
//
// Created by Max Norfolk on 10/15/23.


#pragma once

#include <string>
#include <vector>
#include <experimental/filesystem>


#include "core/fishstore.h"
#include "device/null_disk.h"


typedef fishstore::device::NullDisk disk_t;
typedef fishstore::adapter::SIMDJsonAdapter adapter_t;
using store_t = fishstore::core::FishStore<disk_t, adapter_t>;


std::unique_ptr<store_t> storeFromFile(const std::string &file_path, int &psf_id) {
    static int counter = 0;
    const std::string fishstore_file_name = "fishstore_out_" + std::to_string(counter++);
    const size_t store_size = 1UL << 31;
    const size_t table_size = 1UL << 24;


    std::vector<std::string> batches;
    std::ifstream in(file_path);

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
    }

    printf("everything ingested\n");

    std::experimental::filesystem::create_directory(fishstore_file_name);
    auto store = std::make_unique<store_t>(table_size, store_size, fishstore_file_name);


    // add PSF
    store->StartSession();
    std::vector<ParserAction> parser_actions;
    PsfLookup psf = store->MakeEzPsf("(Int) id");
    parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
    store->ApplyParserShift(parser_actions, [](uint64_t safe_address) {});
    store->CompleteAction(true);


    // Load Data
    for (const auto &batch: batches) {
        store->BatchInsert(batch, 1);
    }
    store->Refresh();

    return store;
}