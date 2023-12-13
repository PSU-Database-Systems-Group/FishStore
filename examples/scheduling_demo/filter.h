// Selections
//
// Created by Max Norfolk on 12/12/23.


#pragma once

#include <functional>
#include <utility>
#include "types.h"

class Filter {
public:
    typedef std::function<bool(StringRef)> FilterFunction;

    Filter(FilterFunction func, const std::vector<int32_t> &fieldIds)
            : func(std::move(func)), field_ids(fieldIds) {}

    // checks the record given the PSF that has already checked it
    bool check(PsfId id, StringRef record) {
        // TODO use id to fix it
        return func(record);
    }

private:
    FilterFunction func;
    std::vector<int32_t> field_ids;
};