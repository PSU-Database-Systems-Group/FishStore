// A simple header that contains the required record/field accessing functions that can be linked into the JIT system
// to access fields from a given record.
//
// Created by Max Norfolk on 7/7/23.

#pragma once

#include <cstdint>

namespace fishstore::ezpsf {
    typedef void EzRecord;

    struct StringRef {
        const char *data;
        uint64_t size;
    };
    namespace type_conversion {
        template<typename T>
        struct GenericNullable {
            bool hasValue;
            T value;
        };

        typedef GenericNullable<bool> NullableBool;
        typedef GenericNullable<int32_t> NullableInt32;
        typedef GenericNullable<int64_t> NullableInt64;
        typedef GenericNullable<double> NullableDouble;
        typedef GenericNullable<StringRef> NullableStringRef;

        // for a given Adapter
        template<typename A>
        class TypeConversion {
            typedef std::vector<typename A::field_t> record_t;

            static void getBool(EzRecord *record, uint64_t field_identifier, NullableBool *return_ptr) {
                auto rec = (record_t *) record;
                auto &field = rec->at(field_identifier);
                auto val = field.GetAsBool();
                *return_ptr = {val.HasValue(), val.Value()};
            }

            static void getInt32(EzRecord *record, uint64_t field_identifier, NullableInt32 *return_ptr) {
                auto rec = (record_t *) record;
                auto &field = rec->at(field_identifier);
                auto val = field.GetAsInt();
                *return_ptr = {val.HasValue(), val.Value()};
            }

            static void getInt64(EzRecord *record, uint64_t field_identifier, NullableInt64 *return_ptr) {
                auto rec = (record_t *) record;
                auto &field = rec->at(field_identifier);
                auto val = field.GetAsLong();
                *return_ptr = {val.HasValue(), val.Value()};
            }

            static void getDouble(EzRecord *record, uint64_t field_identifier, NullableDouble *return_ptr) {
                auto rec = (record_t *) record;
                auto &field = rec->at(field_identifier);
                auto val = field.GetAsDouble();
                *return_ptr = {val.HasValue(), val.Value()};
            }

            static void getStringRef(EzRecord *record, uint64_t field_identifier, NullableStringRef *return_ptr) {
                auto rec = (record_t *) record;
                auto &field = rec->at(field_identifier);
                auto val = field.GetAsStringRef();
                StringRef ref = {val.Value().Data(), val.Value().Length()};
                *return_ptr = {val.HasValue(), ref};
            }
        };
    }
}