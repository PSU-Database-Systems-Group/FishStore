// A small demo showing the performance gains when we can utilize PSFs versus just doing a basic scan
//
// Created by Max Norfolk on 9/28/23.



#include "scan_scheduler.h"
#include "scan_contexts.h"
#include "misc.h"

// main method
int main(int argc, char **argv) {
    auto pair = storeFromFile("/scratch/mnorfolk/FishStore/src/adapters/all.json", {
            PsfAction{"(Str) type == \"PushEvent\"", 0, 500000}
    });
    auto store = std::move(pair.first);
    auto map = std::move(pair.second);

    auto plan = plan_scans(store.get(), map);
    execute_scans(store.get(), plan);


    /* auto tweets = storeFromFile("/scratch/mnorfolk/Data/json/srilanka_floods_2017.json", {
             {0,    [&full_psf_id](store_t *store) {
                 // add PSF
                 std::vector<ParserAction> parser_actions;
                 PsfLookup psf = store->MakeEzPsf("(Str) class_label == \"not_humanitarian\"");
                 parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
                 store->ApplyParserShift(parser_actions, [](uint64_t s_addr) {

                 });
                 store->CompleteAction(true);
                 full_psf_id = psf.id;
             }},
             {3637248, [&partial_psf_id, &partial_reg_addr](store_t *store) {
                 // add PSF
                 std::vector<ParserAction> parser_actions;
                 PsfLookup psf = store->MakeEzPsf("(Str) class_label == \"not_humanitarian\"");
                 parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
                 store->ApplyParserShift(parser_actions, [&partial_reg_addr](uint64_t s_addr) {
                     partial_reg_addr = s_addr;
                 });
                 store->CompleteAction(true);
                 partial_psf_id = psf.id;
             }}
     });*/

/*    PsfMap map;
    map.insert({1, {1, 0, 25, 0}});
    map.insert({2, {2, 30, 80, 4}});
    map.insert({3, {3, 50, 60, 0}});
    map.insert({4, {4, 60, 120, 3}});
    map.insert({5, {5, 40, 50, 5}});
    schedule2(nullptr, map, 0, 100);*/



    /*
    uint32_t full_psf_id;

    uint32_t partial_psf_id;
    uint64_t partial_reg_addr;

    // testing basic scan with filter
    auto tweets = storeFromFile("/scratch/mnorfolk/Data/json/srilanka_floods_2017.json", {
            {0,    [&full_psf_id](store_t *store) {
                // add PSF
                std::vector<ParserAction> parser_actions;
                PsfLookup psf = store->MakeEzPsf("(Str) class_label == \"not_humanitarian\"");
                parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
                store->ApplyParserShift(parser_actions, [](uint64_t s_addr) {

                });
                store->CompleteAction(true);
                full_psf_id = psf.id;
            }},
            {3637248, [&partial_psf_id, &partial_reg_addr](store_t *store) {
                // add PSF
                std::vector<ParserAction> parser_actions;
                PsfLookup psf = store->MakeEzPsf("(Str) class_label == \"not_humanitarian\"");
                parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
                store->ApplyParserShift(parser_actions, [&partial_reg_addr](uint64_t s_addr) {
                    partial_reg_addr = s_addr;
                });
                store->CompleteAction(true);
                partial_psf_id = psf.id;
            }}
    });

    expr_func<std::string, adapter_t> func = [](expr_func_args<adapter_t> args) {
        if (!args.empty()) {
            auto value = args[0].GetAsString();
            if (value.HasValue() && value.Value() == "not_humanitarian")
                return Nullable<std::string>(value.Value());
        }
        return Nullable<std::string>(); // return null
    };


    auto callback = [](IAsyncContext *ctxt, Status result) {
        assert(result == Status::Ok);
    };
    {
        printf("Starting PSF Scan: ");
        auto start = std::chrono::steady_clock::now();
        JsonInlineScanContext ez_psf_ctx(full_psf_id, 1);
        tweets->Scan(ez_psf_ctx, callback, 1);
        tweets->CompletePending(true);
        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (fullscan)\n"
                  << std::endl;
    }
    {
        printf("Starting Full Scan: 1 Shot ");
        auto start = std::chrono::steady_clock::now();
        AdapterFullScanContext<std::string, adapter_t> scan_context({"class_label"}, func);
        tweets->FullScan(scan_context, callback, 1);
        tweets->CompletePending(true);
        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (fullscan)\n"
                  << std::endl;
    }
    {
        printf("Starting Full Scan with PSF ");
        auto start = std::chrono::steady_clock::now();

        JsonInlineScanContext ez_psf_ctx(partial_psf_id, 1);
        tweets->Scan(ez_psf_ctx, callback, 1);
        tweets->CompletePending(true);

        AdapterFullScanContext<std::string, adapter_t> scan_context({"class_label"}, func);
        tweets->FullScan(scan_context, callback, 1, 0, partial_reg_addr - 1);
        tweets->CompletePending(true);
        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (fullscan)\n"
                  << std::endl;
    }*/
}

/*    int job_psf;
    int people_psf;

    auto jobs = storeFromFile("/scratch/mnorfolk/ClionProjects/FishStore/examples/scheduling_demo/jobs.json",
                              job_psf);
    auto people = storeFromFile("/scratch/mnorfolk/ClionProjects/FishStore/examples/scheduling_demo/people.json",
                                people_psf);


    expr_func<int, adapter_t> func = [](expr_func_args<adapter_t> args) {
        if (!args.empty()) {
            auto value = args[0].GetAsInt();
            if (value.HasValue())
                return Nullable<int>(value.Value() % 2);
        }

        return Nullable<int>(); // return null
    };


    AdapterFullScanContext<int, adapter_t> scan_context({"id"}, func);
    auto callback = [](IAsyncContext *ctxt, Status result) {
        assert(result == Status::Ok);
    };

*//*    auto start = std::chrono::steady_clock::now();
    people->FullScan(scan_context, callback, 1);
    people->CompletePending(true);
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (fullscan)"
              << std::endl;*//*
*//*

    start = std::chrono::steady_clock::now();
    JsonInlineScanContext ez_psf_ctx(people_psf, 1);
    people->Scan(ez_psf_ctx, callback, 1);
    people->CompletePending(true);
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (psf)" << std::endl;
*//*

    *//*{
        FullScanAdapterJoinContext<disk_t, adapter_t, BadInnerJoinContext<adapter_t>> join_ctx({"id"}, jobs.get());
        std::cout << "\nStarting FullScan with BadInnerJoin" << std::endl;
        auto start = std::chrono::steady_clock::now();
        people->FullScan(join_ctx, callback, 1);
        people->CompletePending(true);
        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (fullscan)"
                  << std::endl;
    }
    {
        ScanAdapterJoinContext<disk_t, adapter_t, HashInnerJoinContext> join_ctx({"id"}, jobs.get(), job_psf);
        std::cout << "\nStarting FullScan with HashInnerJoin" << std::endl;
        auto start = std::chrono::steady_clock::now();
        people->FullScan(join_ctx, callback, 1);
        people->CompletePending(true);
        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (fullscan)"
                  << std::endl;
    }*//*

    people->StopSession();
    jobs->StopSession();*/