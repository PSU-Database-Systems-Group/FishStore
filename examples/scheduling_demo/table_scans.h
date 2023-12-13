// Decides how to order the PSF Scans & Full Scans based upon the PSFs
//
// Created by Max Norfolk on 10/29/23.


#pragma once

#include "misc.h"
#include "scan_contexts.h"
#include "filter.h"

// plans the scans prioritizing psf scans
// returns: the PSF info's that should be used for completing the scans.
// the start and end timestamps for these PSF infos will be different from the maximum allowed scans,
// because if a PSF is only partially scanned, then we only use that partial scan.
//
// see execute_scans to actually execute the scans.
std::vector<PsfInfo>
plan_scans(store_t *store, PsfMap map, PsfAddress start = 0, PsfAddress end = Address::kMaxAddress) {
    std::vector<PsfId> psf_ordered;

    for (auto &item: map) {
        item.second.start = std::max(item.second.start, start);
        item.second.end = std::min(item.second.end, end);
    }

    std::map<PsfAddress, bool> all_timestamps;
    all_timestamps.emplace(start, false);
    all_timestamps.emplace(end, false);

    for (const auto &item: map) {
        psf_ordered.emplace_back(item.first);
        all_timestamps.emplace(item.second.start, false);
        all_timestamps.emplace(item.second.end, false);
    }
    printf("Discrete-alized Time [");
    for (const auto &item: all_timestamps)
        printf("%lu, ", item.first);
    printf("]\n");

    // order it so the first to use is at the end, and we slowly pop things off the back
    std::sort(psf_ordered.begin(), psf_ordered.end(), [&map](uint32_t id1, uint32_t id2) {
        auto info1 = map.at(id1);
        auto info2 = map.at(id2);

        if (info1.preference == info2.preference) {
            auto len1 = info1.end - info1.start;
            auto len2 = info2.end - info2.start;
            if (len1 < len2)
                return true;
        }
        return info1.preference < info2.preference;
    });


    std::vector<PsfInfo> used_info;
    while (!psf_ordered.empty()) {
        auto back = psf_ordered.back();
        psf_ordered.pop_back();

        auto info = map.at(back);

        auto it = all_timestamps.find(info.start);
        auto it_end = all_timestamps.find(info.end);
        while (it != it_end) {
            while (it->second == true) {
                it++;
                if (it == it_end)
                    goto DOUBLE_BREAK;
            }
            auto scan_start = it->first;
            while (it->second == false) {
                it->second = true;
                it++;
                if (it == it_end)
                    break;
            }
            auto scan_end = it->first;

            used_info.emplace_back(info.id, scan_start, scan_end, info.fallback);
        }
        DOUBLE_BREAK:;
    }

    printf("Finished assigning PSF scans\n");

    auto fs_it = all_timestamps.begin();
    auto fs_it_end = all_timestamps.find(end);
    while (fs_it != fs_it_end) {
        // find first missing value
        while (fs_it->second) {
            fs_it++;

            // everything covered
            if (fs_it == all_timestamps.end())
                goto FS_DOUBLE_BREAK;
        }
        PsfAddress start_addr = fs_it->first;

        // fill missing value until there is a filled value
        while (!fs_it->second) {
            printf("Covering: %lu\n", fs_it->first);
            fs_it->second = true;
            fs_it++;
            if (fs_it == fs_it_end) {
                printf("breaking out of loop");
                break;
            }
        }

        PsfAddress end_addr = fs_it->first;
        used_info.emplace_back(FS_ID, start_addr, end_addr, nullptr);
    }

    FS_DOUBLE_BREAK:

    printf("Discrete-alized Time [");
    for (const auto &item: all_timestamps)
        printf("[%lu, %d], ", item.first, item.second);
    printf("]\n");


    printf("Finished assigning remaining timestamps\n");

    for (const auto &item: used_info) {
        if (item.id == FS_ID) {
            printf("FullScan: [%lu, %lu]\n", item.start, item.end);
        } else
            printf("PSF '%2d': [%lu, %lu]\n", item.id, item.start, item.end);
    }
    return used_info;
}

void execute_scans(store_t *store, const std::vector<PsfInfo> &plan, const Filter& secondary_filter) {
    // callback
    AsyncCallback callback = [](fishstore::core::IAsyncContext *, fishstore::core::Status status) {
        assert(status == fishstore::core::Status::Ok);
    };

    // execute each scan
    for (const auto &item: plan) {
        if (item.id == FS_ID) { // FullScan
            PlanFullScanContext fs_ctx(secondary_filter);
            store->FullScan(fs_ctx, callback, 1);
        } else { // regular scan
            PlanScanContext scan_ctx(item.id, 1, secondary_filter);
            store->Scan(scan_ctx, callback, 1);
        }
    }
}