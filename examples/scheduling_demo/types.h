// All the file types
//
// Created by Max Norfolk on 12/12/23.


#pragma once

#include <functional>
#include <map>

#define SCALE_FACTOR 1
#define MAX_RECORDS 10000

typedef fishstore::device::NullDisk disk_t;
typedef fishstore::adapter::SIMDJsonAdapter adapter_t;
using store_t = fishstore::core::FishStore<disk_t, adapter_t>;


typedef fishstore::adapter::StringRef StringRef;

typedef uint64_t PsfAddress;
typedef uint32_t PsfId;

// maps fishstore Record => nullable int
typedef std::function<fishstore::adapter::NullableInt(StringRef)> PsfFallback;



constexpr PsfId FS_ID = -1;

struct PsfInfo {
    PsfInfo(PsfId id, PsfFallback fallback) : id(id), fallback(std::move(fallback)) {}

    PsfInfo(PsfId id, PsfAddress start, PsfAddress end, PsfFallback fallback)
            : id(id), start(start), end(end), preference(0), fallback(std::move(fallback)) {}

    PsfInfo(PsfId id, PsfAddress start, PsfAddress end, int selectivity) : id(id), start(start), end(end),
                                                                           preference(selectivity) {}

    PsfId id;
    PsfAddress start;
    PsfAddress end;
    int preference; // prefer higher preference for psfs scans
    PsfFallback fallback;
};

// Maps PSF id to PsfInfo
typedef std::unordered_map<PsfId, PsfInfo> PsfMap;

typedef std::unordered_map<std::string, PsfId> FieldMap;