// A small demo showing the performance gains when we can utilize PSFs versus just doing a basic scan
//
// Created by Max Norfolk on 9/28/23.


#include <string>
#include <vector>
#include <experimental/filesystem>


#include "adapters/simdjson_adapter.h"
#include "core/fishstore.h"
#include "device/null_disk.h"

#include "scan_contexts.h"

typedef fishstore::device::NullDisk disk_t;
typedef fishstore::adapter::SIMDJsonAdapter adapter_t;
using store_t = fishstore::core::FishStore<disk_t, adapter_t>;

// main method
int main(int argc, char **argv) {
    const std::string file_path = "/scratch/mnorfolk03/FishStore/src/adapters/all.json";
    const std::string fishstore_file_name = "fishstore_out";
    const size_t store_size = 1UL << 31;
    const size_t table_size = 1UL << 24;


    std::vector<std::string> batches;
    std::ifstream in(file_path);

    std::string line;
    size_t json_batch_count, line_count, json_batch_size, record_count;
    json_batch_count = line_count = json_batch_size = record_count = 0;

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

    std::experimental::filesystem::create_directory(fishstore_file_name);
    store_t store{table_size, store_size, fishstore_file_name};

    printf("Finish initializing FishStore, starting the demo...\n");
    store.StartSession();

    bool finished = false;

    std::thread main_thread([&]() {
        store.StartSession();
        while (!finished)
            for (const auto &item: batches) {
                if (finished)
                    return;
                store.BatchInsert(item, 1);
                store.Refresh();
            }
    });
    printf("stalling\n");
    sleep(1);
    printf("starting full scan\n");

    expr_func<int, adapter_t> func = [](expr_func_args<adapter_t> args) {
        auto value = args[0].GetAsInt();
        if (value.HasValue())
            return Nullable<int>(value.Value() % 2);
        return Nullable<int>(); // return null
    };
    AdapterFullScanContext<int, adapter_t> scan_context({"id"}, func);
    auto callback = [](IAsyncContext *ctxt, Status result) {
        assert(result == Status::Ok);
    };

    store.FullScan(scan_context, callback, 1);
    sleep(10);

    printf("done\n");

    finished = true;
    main_thread.join();
    store.StopSession();
}