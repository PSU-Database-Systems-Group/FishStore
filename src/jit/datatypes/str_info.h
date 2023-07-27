// String type operations
//
// Created by Max Norfolk on 6/3/23.


#pragma once

#include "jit/datatypes/conversion_declaration.h"

namespace fishstore::ezpsf::str_info {

    // negative if s1 < s2. 0 if s1 == s2. positive if s1 > s2.
    int FUNC_STR_CMP(StringRef s1, StringRef s2) {
        uint64_t num_to_cmp = std::min(s1.size, s2.size);
        auto result = std::strncmp(s1.data, s2.data, num_to_cmp);

        // if they are equal, verify the size is equal. Otherwise, return based upon the size, such that the smaller
        // is smaller than larger
        if (result == 0)
            return static_cast<int64_t>(s1.size) - static_cast<int64_t>(s2.size);
        return result;
    }

    // returns the size as an uint64_t. This is unused as it can be accessed by the struct
    inline uint64_t FUNC_STR_LEN(StringRef s) {
        return s.size;
    }

    llvm::StringRef REF_STR_CMP = "@__STRING_INFO_CMP";
    llvm::StringRef REF_STR_LEN = "@__STRING_INFO_LEN";
}