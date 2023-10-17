// A small demo showing the performance gains when we can utilize PSFs versus just doing a basic scan
//
// Created by Max Norfolk on 9/28/23.



#include "scan_contexts.h"
#include "misc.h"


// main method
int main(int argc, char **argv) {
    int job_psf;
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

/*    auto start = std::chrono::steady_clock::now();
    people->FullScan(scan_context, callback, 1);
    people->CompletePending(true);
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (fullscan)"
              << std::endl;*/
/*

    start = std::chrono::steady_clock::now();
    JsonInlineScanContext ez_psf_ctx(people_psf, 1);
    people->Scan(ez_psf_ctx, callback, 1);
    people->CompletePending(true);
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms (psf)" << std::endl;
*/

    {
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
    }

    people->StopSession();
    jobs->StopSession();
}