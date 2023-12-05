// Old versions of code that might still be useful
//
// Created by Max Norfolk on 11/12/23.


#pragma once

#include "misc.h"

#define LEG_SCALE_FACTOR 1
#define LEG_MAX_RECORDS 800000

namespace legacy {


// map argument will map a number of records to insert, before registering the PSF by calling the associated function
    std::unique_ptr<store_t>
    storeFromFile(const std::string &file_path,
                  const std::multimap<uint64_t, std::function<void(store_t *)>> &psf_map) {
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
            if (batches.size() >= LEG_MAX_RECORDS)
                break;
        }

        auto batches_copy(batches);
        for (int i = 1; i < LEG_SCALE_FACTOR; ++i) {
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

    /////////////////////////////////////////////////////
    // Main Methods
    /////////////////////////////////////////////////////

    int main(int argc, char **argv) {
        printf("/////////////////////////////////\n// Start Legacy\n/////////////////////////////////\n\n");

        uint32_t full_psf_id = 0;

        uint32_t first_psf_id = 0;
        uint64_t first_reg_addr = 0;

        uint32_t second_psf_id = 0;
        uint64_t second_reg_addr = 0;

        uint64_t middle_addr = 0;


        std::multimap<uint64_t, std::function<void(store_t *)>> psf_map = {
                {0, [&full_psf_id](store_t *store) {
                    // add PSF
                    std::vector<ParserAction> parser_actions;
                    PsfLookup psf = store->MakeEzPsf("(Int) actor.id % 2");
                    parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
                    store->ApplyParserShift(parser_actions, [](uint64_t s_addr) {

                    });
                    store->CompleteAction(true);
                    full_psf_id = psf.id;
                }},
                {LEG_MAX_RECORDS * (1.0 / 3), [&first_psf_id, &first_reg_addr](store_t *store) {
                    // add PSF
                    std::vector<ParserAction> parser_actions;
                    PsfLookup psf = store->MakeEzPsf("(Int) actor.id % 2");
                    parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
                    store->ApplyParserShift(parser_actions, [&first_reg_addr](uint64_t s_addr) {
                        first_reg_addr = s_addr;
                    });
                    store->CompleteAction(true);
                    first_psf_id = psf.id;
                }},
                // middle PSF
                {LEG_MAX_RECORDS * (0.5),     [&middle_addr](store_t *store) {
                    std::vector<ParserAction> parser_actions;
                    PsfLookup psf = store->MakeEzPsf("(Int) actor.id % 2");
                    parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
                    store->ApplyParserShift(parser_actions, [&middle_addr](uint64_t s_addr) {
                        middle_addr = s_addr;
                    });
                    store->CompleteAction(true);
                }},
                {LEG_MAX_RECORDS * (2.0 / 3), [&second_psf_id, &second_reg_addr](store_t *store) {
                    // add PSF
                    std::vector<ParserAction> parser_actions;
                    PsfLookup psf = store->MakeEzPsf("(Int) actor.id % 2");
                    parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
                    store->ApplyParserShift(parser_actions, [&second_reg_addr](uint64_t s_addr) {
                        second_reg_addr = s_addr;
                    });
                    store->CompleteAction(true);
                    second_psf_id = psf.id;
                }
                }};

        auto tweets = storeFromFile("/scratch/mnorfolk/FishStore/src/adapters/all.json", psf_map);

        expr_func<int, adapter_t> func = [](expr_func_args<adapter_t> args) {
            if (!args.empty()) {
                auto value = args[0].GetAsInt();
                if (value.HasValue() && value.Value() % 2)
                    return Nullable<int>(value.Value());
            }
            return Nullable<int>(); // return null
        };


        auto callback = [](IAsyncContext *ctxt, Status result) {
            assert(result == Status::Ok);
        };


        {
            printf("Starting PSF Scan: | ");
            auto start = std::chrono::steady_clock::now();
            JsonInlineScanContext ez_psf_ctx(full_psf_id, 1);
            tweets->Scan(ez_psf_ctx, callback, 1);
            tweets->CompletePending(true);
            auto end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " ms (psf scan)\n"
                      << std::endl;
        }
        {
            printf("Starting Full Scan: 1 Shot | ");
            auto start = std::chrono::steady_clock::now();
            AdapterFullScanContext<int, adapter_t> scan_context({"actor.id"}, func);
            tweets->FullScan(scan_context, callback, 1);
            tweets->CompletePending(true);
            auto end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " ms (fullscan)\n"
                      << std::endl;
        }
        {
            printf("Starting Partial PSF 1/3 FS, PSF starts at ideal | ");
            auto start = std::chrono::steady_clock::now();
            JsonInlineScanContext ez_psf_ctx(first_psf_id, 1);
            tweets->Scan(ez_psf_ctx, callback, 1);
            tweets->CompletePending(true);

            AdapterFullScanContext<int, adapter_t> scan_context({"actor.id"}, func);
            tweets->FullScan(scan_context, callback, 1, 0, first_reg_addr - 1);
            tweets->CompletePending(true);
            auto end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " ms (fullscan)\n"
                      << std::endl;
        }
        {
            printf("Starting Partial PSF 1/3 FS, PSF starts at 0 | ");
            auto start = std::chrono::steady_clock::now();
            JsonInlineScanContext ez_psf_ctx(full_psf_id, 1);
            tweets->Scan(ez_psf_ctx, callback, 1, first_reg_addr);
            tweets->CompletePending(true);

            AdapterFullScanContext<int, adapter_t> scan_context({"actor.id"}, func);
            tweets->FullScan(scan_context, callback, 1, 0, first_reg_addr - 1);
            tweets->CompletePending(true);
            auto end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " ms (fullscan)\n"
                      << std::endl;
        }
        {
            printf("Starting Overlapping 1/2, prior 1st half | ");
            auto start = std::chrono::steady_clock::now();
            JsonInlineScanContext ez_psf_ctx(full_psf_id, 1);
            tweets->Scan(ez_psf_ctx, callback, 1, 0, middle_addr - 1);
            tweets->CompletePending(true);

            JsonInlineScanContext ez_psf_ctx2(full_psf_id, 1);
            tweets->Scan(ez_psf_ctx2, callback, 1, middle_addr);
            tweets->CompletePending(true);

            auto end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " ms (fullscan)\n"
                      << std::endl;
        }
        {
            printf("Starting Overlapping 1/2, prior 2nd half | ");
            auto start = std::chrono::steady_clock::now();
            JsonInlineScanContext ez_psf_ctx(full_psf_id, 1);
            tweets->Scan(ez_psf_ctx, callback, 1, 0, first_reg_addr - 1);
            tweets->CompletePending(true);

            JsonInlineScanContext ez_psf_ctx2(first_psf_id, 1);
            tweets->Scan(ez_psf_ctx2, callback, 1, first_reg_addr);
            tweets->CompletePending(true);

            auto end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " ms (fullscan)\n"
                      << std::endl;
        }
        /*{
            printf("Starting Full Scan with PSF ");
            auto start = std::chrono::steady_clock::now();

            JsonInlineScanContext ez_psf_ctx(partial_psf_id, 1);
            tweets->Scan(ez_psf_ctx, callback, 1);
            tweets->CompletePending(true);

            AdapterFullScanContext<std::string, adapter_t> scan_context({"class_label"}, func);
            tweets->FullScan(scan_context, callback, 1, 0, partial_reg_addr - 1);
            tweets->CompletePending(true);
            auto end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " ms (fullscan)\n"
                      << std::endl;
        }*/
        printf("/////////////////////////////////\n//  End  Legacy\n/////////////////////////////////\n\n");

        return 0;
    }

}