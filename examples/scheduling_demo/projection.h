// Projection operator
//
// Created by Max Norfolk on 12/13/23.


#pragma once

#include <string>
#include <utility>
#include <vector>
#include "record_helper.h"

#include "adapters/simdjson_adapter.h"

namespace plan {

    template<typename A>
    class Projection {
        typedef fishstore::adapter::SIMDJsonAdapter adapter_t;
        typedef typename A::parser_t parser_t;
        typedef typename A::field_t field_t;
        typedef typename A::record_t record_t;

        Projection(std::vector<std::string> fields, const FieldMap &map)
                : fields(std::move(fields)), field_map(map), id_to_field() {
        }

        std::vector<Value> next(Record *record) {
            return {};
        }

    private:
        std::vector<std::string> fields;
        const FieldMap &field_map;
    };
}