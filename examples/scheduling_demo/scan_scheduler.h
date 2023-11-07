// Decides how to order the PSF Scans & Full Scans based upon the PSFs
//
// Created by Max Norfolk on 10/29/23.


#pragma once

#include "misc.h"
#include "scan_contexts.h"

// COUNT(*) FROM ... WHERE ...
void schedule(store_t *store, const PsfMap &map,
              PsfTimestamp start = 0, PsfTimestamp end = Address::kMaxAddress) {

    std::map<PsfTimestamp, uint32_t> start_time;
    std::map<PsfTimestamp, uint32_t> end_time;
    for (const auto &item: map) {
        start_time.emplace(item.second.start, item.first);
        end_time.emplace(item.second.end, item.first);
    }

    auto s_it = start_time.begin();
    auto e_it = end_time.begin();

    std::vector<PsfInfo> used_info;

    while (start != end) {
        if (s_it == start_time.end())
            break;

        uint32_t psf = s_it->second;
        PsfInfo info = map.at(psf);

        // if it ends before this one starts, skip
        if (info.end < start) {
            s_it++;
        } else if (s_it->first <= start) {
            info.start = start;
            used_info.push_back(info);
            start = info.end; // new start is where this one ends

            s_it++;
        } else {
            uint32_t future_psf = s_it->second;
            PsfInfo future_info = map.at(future_psf);
            PsfTimestamp full_scan_end = future_info.start;
            PsfInfo full_scan = {0, start, full_scan_end};
            used_info.push_back(full_scan);

            start = full_scan_end;
        }
    }

    // failsafe
    if (start != end) {
        PsfInfo full_scan = {0, start, end};
        used_info.push_back(full_scan);
    }

    printf("\n\n\nEnd:\n");

    for (const auto &item: used_info) {
        if (item.id == 0) {
            printf("FullScan: [%lu, %lu]\n", item.start, item.end);
        } else
            printf("PSF '%2d': [%lu, %lu]\n", item.id, item.start, item.end);
    }
}

// plans the scans prioritizing psf scans
// returns: the PSF info's that should be used for completing the scans.
// the start and end timestamps for these PSF infos will be different from the maximum allowed scans,
// because if a PSF is only partially scanned, then we only use that partial scan.

// see execute_scans to actually execute the scans.
std::vector<PsfInfo>
plan_scans(store_t *store, PsfMap map, PsfTimestamp start = 0, PsfTimestamp end = Address::kMaxAddress) {
    std::vector<uint32_t> psf_ordered;

    for (auto &item: map) {
        item.second.start = std::max(item.second.start, start);
        item.second.end = std::min(item.second.end, end);
    }

    std::map<PsfTimestamp, bool> all_timestamps;
    all_timestamps.emplace(start, false);
    all_timestamps.emplace(end, false);

    for (const auto &item: map) {
        psf_ordered.emplace_back(item.first);
        all_timestamps.emplace(item.second.start, false);
        all_timestamps.emplace(item.second.end, false);
    }

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

            used_info.emplace_back(info.id, scan_start, scan_end);
        }
        DOUBLE_BREAK:;
    }
    auto fc_it = all_timestamps.begin();
    auto fc_it_end = all_timestamps.find(end);
    while (fc_it != fc_it_end) {
        while (fc_it->second) {
            fc_it++;
            if (fc_it == fc_it_end)
                goto FS_DOUBLE_BREAK;
        }
        auto fc_start = fc_it->first;
        while (!fc_it->second) {
            fc_it++;
            if (fc_it == all_timestamps.end())
                break;
        }
        auto fc_end = fc_it->first;

        used_info.emplace_back(0, fc_start, fc_end);
    }

    FS_DOUBLE_BREAK:
    for (const auto &item: used_info) {
        if (item.id == 0) {
            printf("FullScan: [%lu, %lu]\n", item.start, item.end);
        } else
            printf("PSF '%2d': [%lu, %lu]\n", item.id, item.start, item.end);
    }
    return used_info;
}

void execute_scans(store_t *store, std::vector<PsfInfo> plan) {

}