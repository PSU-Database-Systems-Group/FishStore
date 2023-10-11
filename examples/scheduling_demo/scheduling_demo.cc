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
    const std::string file_path = "/scratch/mnorfolk/FishStore/src/adapters/all.json";
    const std::string fishstore_file_name = "fishstore_out";
    const size_t store_size = 1UL << 31;
    const size_t table_size = 1UL << 24;


    std::vector<std::string> batches;
    std::ifstream in(file_path);

    std::string line;
    size_t json_batch_count, line_count, json_batch_size, record_count;
    json_batch_count = line_count = record_count = 0;
    json_batch_size = 1;

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
    store_t store{table_size, store_size, fishstore_file_name};
    store.StartSession();

    // add PSF
    std::vector<ParserAction> parser_actions;
    PsfLookup psf = store.MakeEzPsf("(Int) actor.id % 2");
    parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
    store.ApplyParserShift(parser_actions, [](uint64_t safe_address) {});
    store.CompleteAction(true);


    // done adding PSF

    printf("Finish initializing FishStore with PSF, starting the demo...\n");

    printf("Total number: %ld\n", batches.size());

    for (int i = 0; i < 200000; ++i) {
        store.BatchInsert(batches[i], 1);
    }


    printf("done filling store\n");
    store.Refresh();


    expr_func<int, adapter_t> func = [](expr_func_args<adapter_t> args) {
        if (!args.empty()) {
            auto value = args[0].GetAsInt();
            if (value.HasValue())
                return Nullable<int>(value.Value() % 2);
        }

        return Nullable<int>(); // return null
    };


    AdapterFullScanContext<int, adapter_t> scan_context({"actor.id"}, func);
    auto callback = [](IAsyncContext *ctxt, Status result) {
        assert(result == Status::Ok);
    };

    auto start = std::chrono::steady_clock::now();
    store.FullScan(scan_context, callback, 1);
    store.CompletePending(true);
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (fullscan)" << std::endl;

    start = std::chrono::steady_clock::now();
    JsonInlineScanContext ez_psf_ctx(psf.id, 1);
    store.Scan(ez_psf_ctx, callback, 1);
    store.CompletePending(true);
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (psf)" << std::endl;


    store.StopSession();
}