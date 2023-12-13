//
//
// Created by Max Norfolk on 12/13/23.


#pragma once

#include <map>
#include <utility>
#include "core/record.h"
#include "types.h"
#include "jit/datatypes/types.h"


namespace plan {
    union Value {
        StringRef string;
        int32_t int32;

        Value() : string(StringRef(nullptr, 0)) {}

        explicit Value(const StringRef &string) : string(string) {}

        explicit Value(const char *ptr, size_t size) : string(StringRef(ptr, size)) {}

        explicit Value(int32_t int32) : int32(int32) {}

        explicit Value(uint32_t int32) : int32(int32) {}
    };
    namespace record {
        typedef fishstore::core::Record Record;
        typedef fishstore::ezpsf::DataType DataType;


        class FieldExtract {

            struct FieldData {
                FieldData(int index, const std::string &fieldName) : index(index), field_name(fieldName) {}

                int index;
                std::string field_name;
            };

            // pair of field names & their corresponding data types
            FieldExtract(std::vector<std::pair<std::string, DataType>> fields_, const FieldMap &map)
                    : fields(std::move(fields_)), map(map) {

                for (int i = 0; i < this->fields.size(); ++i) {
                    PsfId id = map.at(fields[i].first);
                    id_to_field.emplace(id, FieldData{i, fields[i].first});
                }
            }

            static constexpr uint32_t GENERAL = 0;
            static constexpr uint32_t INLINE = 1;

            // extracts the fields from the
            std::vector<Value> apply(Record *record) {
                int num_fields = fields.size();

                std::vector<Value> ret;
                ret.resize(num_fields);

                std::vector<bool> covered_fields;
                covered_fields.resize(num_fields);
                int num_covered_fields;


                const auto ptrs = record->header.ptr_cnt;
                for (int i = 0; i < ptrs; ++i) {
                    auto kp = record->get_ptr(i);
                    if (kp->mode == GENERAL) {
                        auto id = kp->general_psf_id;
                        auto it = id_to_field.find(id);
                        if (it == id_to_field.end()) {
                            continue; // don't need this PSF id
                        }
                        auto index = it->second.index;
                        ret[index] = Value(kp->get_value(), kp->value_size);
                        covered_fields[index] = true;
                        num_covered_fields++;
                    } else {
                        assert(kp->mode == INLINE);
                        auto id = kp->inline_psf_id;
                        auto it = id_to_field.find(id);
                        if (it == id_to_field.end()) {
                            continue; // don't need this PSF id
                        }
                        auto index = it->second.index;
                        ret[index] = Value(kp->value);
                        covered_fields[index] = true;
                        num_covered_fields++;
                    }
                }

                // found all fields, don't need to parse anything
                if (num_fields == num_covered_fields)
                    return ret;

                // parse leftover
                std::vector<std::string> parsed_fields;
                std::vector<int> parsed_fields_index;
                for (int i = 0; i < num_fields; ++i) {
                    if (covered_fields[i])
                        continue;

                    parsed_fields.emplace_back(fields[i].first);
                    parsed_fields_index.emplace_back(i);
                }

                // make parser & parse out fields...
            }


            std::vector<std::pair<std::string, DataType>> fields;
            const FieldMap &map;
            std::unordered_map<PsfId, FieldData> id_to_field;
        };

    }
}
