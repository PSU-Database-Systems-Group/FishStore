//
//
// Created by Max Norfolk on 10/15/23.


#pragma once

#include <string>
#include <vector>
#include <experimental/filesystem>


#include "core/fishstore.h"
#include "device/null_disk.h"

#define SCALE_FACTOR (1 << 16)

typedef fishstore::device::NullDisk disk_t;
typedef fishstore::adapter::SIMDJsonAdapter adapter_t;
using store_t = fishstore::core::FishStore<disk_t, adapter_t>;

// map argument will map a number of records to insert, before registering the PSF by calling the associated function
std::unique_ptr<store_t>
storeFromFile(const std::string &file_path, std::multimap<uint64_t, std::function<void(store_t *)>> psf_map) {
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
    }

    auto batches_copy(batches);
    for (int i = 1; i < SCALE_FACTOR; ++i) {
        batches.insert(batches.end(), batches_copy.begin(), batches_copy.end());
    }

    printf("everything ingested\n");

    std::experimental::filesystem::create_directory(fishstore_file_name);
    auto store = std::make_unique<store_t>(table_size, store_size, fishstore_file_name);
    store->StartSession();

    uint64_t i = 0;
    for (auto &it: psf_map) {
        uint64_t needed_i = it.first;
        for (; i < needed_i; ++i) {
            store->BatchInsert(batches[i], 1);
        }
        printf("finished inserting %lu/%lu records before registering psf.\n", i, batches.size());
        store->Refresh();
        it.second(store.get());
    }

    // insert everything remaining
    for (; i < batches.size(); ++i)
        store->BatchInsert(batches[i], 1);
    store->Refresh();


    return store;
}