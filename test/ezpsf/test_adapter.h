// A basic adapter used for testing EzPsfs
//
// Created by Max Norfolk on 7/27/23.


#pragma once

#include "jit/ezpsf.h"

enum class AdapterType {
    Int32,
    StrRef
};

typedef fishstore::ezpsf::StringRef str_t;

using namespace fishstore::ezpsf::type_conversion;

class TestAdapter {
public:
    typedef struct TestField {
    public:
        AdapterType type;
        union {
            int32_t int_val;
            str_t str_val;
        };

        TestField(int32_t val) : type(AdapterType::Int32), int_val(val) {}

        TestField(str_t val) : type(AdapterType::StrRef), str_val(val) {}


        NullableInt32 GetAsInt() {
            return {type == AdapterType::Int32, int_val};
        }

        NullableStringRef GetAsStringRef() {
            return {type == AdapterType::StrRef, str_val};
        }

        NullableBool GetAsBool() {
            return {false, false};
        }

        NullableInt64 GetAsLong() {
            return {false, 0};
        }

        NullableDouble GetAsDouble() {
            return {false, 0.0};
        }
    } field_t;
};

typedef TestAdapter::field_t field_t;
typedef std::vector<field_t> record_t;