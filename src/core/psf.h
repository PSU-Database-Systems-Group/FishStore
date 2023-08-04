// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#pragma once

#include "tsl/hopscotch_map.h"
#include "jit/ezpsf.h"

namespace fishstore {
    namespace core {

        struct NullableInt {
            bool is_null;
            int32_t value;
        };

        static_assert(sizeof(NullableInt) == 8, "sizeof(PSFRetValue) != 8");

        struct NullableStringRef {
            bool is_null = true;
            bool need_free = false;
            uint32_t size = 0;
            const char *payload = nullptr;
        };

        static_assert(sizeof(NullableStringRef) == 16, "sizeof(PSFRetValue) != 16");

// Inline PSF
        template<class A>
        using inline_psf_t = NullableInt(*)(const std::vector<typename A::field_t> &);

// General PSF
        template<class A>
        using general_psf_t = NullableStringRef(*)(const std::vector<typename A::field_t> &);

        constexpr int64_t LIB_PROJECTION = -1;
        constexpr int64_t LIB_EZ_PSF = -2;

        template<class A>
        struct InlinePSF {
            InlinePSF() {
                fields.clear();
                eval_ = nullptr;
            }

            inline NullableInt Eval(
                    const tsl::hopscotch_map<uint16_t, typename A::field_t> &field_map) {
                std::vector<typename A::field_t> args;
                args.reserve(fields.size());
                for (auto &field_id: fields) {
                    auto it = field_map.find(field_id);
                    if (it == field_map.end()) return NullableInt();
                    args.emplace_back(it->second);
                }
                return eval_(args);
            }

            std::vector<uint16_t> fields;
            union {
                inline_psf_t<A> eval_;
                ezpsf::EzPsf ez_eval;
            };
            int64_t lib_id;
            std::string func_name;
        };

        template<class A>
        struct GeneralPSF {
            GeneralPSF() {
                fields.clear();
                eval_ = nullptr;
                lib_id = -1;
                func_name = "";
                ret_type = ezpsf::DataType::ERROR_T;
            }

            inline NullableStringRef Eval(
                    const tsl::hopscotch_map<uint16_t, typename A::field_t> &field_map) {
                std::vector<typename A::field_t> args;
                args.reserve(fields.size());
                for (auto &field_id: fields) {
                    auto it = field_map.find(field_id);
                    if (it == field_map.end()) return NullableStringRef();
                    args.emplace_back(it->second);
                }
                return eval_(args);
            }

            std::vector<uint16_t> fields;
            union {
                general_psf_t<A> eval_;
                ezpsf::EzPsf ez_eval;
            };
            int64_t lib_id;
            std::string func_name;

            ezpsf::DataType ret_type;
        };

        template<class A>
        inline NullableStringRef projection(const std::vector<typename A::field_t> &fields) {
            auto ref = fields[0].GetAsStringRef();
            if (ref.HasValue()) {
                auto val = ref.Value();
                return NullableStringRef{false, false, static_cast<uint32_t>(val.Length()), val.Data()};
            } else return NullableStringRef{};
        }

        enum class PsfType {
            INLINE,
            GENERAL
        };

        template<class A>
        struct AnyPsf {
            union {
                GeneralPSF<A> general_psf;
                InlinePSF<A> inline_psf;
            };
            PsfType type;
        };

    }  // namespace core
}  // namespace fishstore
