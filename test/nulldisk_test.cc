// Some basic tests for NullDisk
//
// Created by Max Norfolk on 10/17/23.

#include <gtest/gtest.h>

#include <utility>


#include "adapters/common_utils.h"
#include "adapters/simdjson_adapter.h"

#include "core/fishstore.h"
#include "device/null_disk.h"

#define NUM_VALUES 4000000

using namespace fishstore::core;
using adapter_t = fishstore::adapter::SIMDJsonAdapter;
using disk_t = fishstore::device::NullDisk;
using store_t = FishStore<disk_t, adapter_t>;


template<typename T>
using Nullable = fishstore::adapter::Nullable<T>;

// A function similar to an EzPsf, but it will be run after the data has already
// been ingested during a Scan
template<typename A>
using expr_func_args = std::vector<typename A::field_t>;

template<typename T, typename A>
using expr_func = std::function<Nullable<T>(expr_func_args<A>)>;


template<typename T, typename A>
class AdapterFullScanContext : public IAsyncContext {
    typedef typename A::parser_t parser_t;
    typedef typename A::record_t record_t;
    typedef typename A::field_t field_t;

    typedef expr_func<T, A> expr_eval;
public:
    AdapterFullScanContext() = delete;

    ~AdapterFullScanContext() override { delete parser; }

    AdapterFullScanContext(const std::vector<std::string> &field_names, expr_eval eval,
                           std::function<void(int)> check_func)
            : parser(A::NewParser(field_names)),
              eval_(std::move(eval)),
              check_count(std::move(check_func)),
              cnt(0) {
    }

    inline void Touch(const char *payload, uint32_t payload_size) {
        ++cnt;
    }

    inline void Finalize() {
        check_count(cnt);
    }

    inline bool check(const char *payload, uint32_t payload_size) {
        parser->Load(payload, payload_size);

        // check to make sure the parser has a value
        bool check = parser->HasNext();
        assert(check);

        record_t rec = parser->NextRecord();
        assert(!parser->HasNext());

        // evaluate
        Nullable<T> ret = eval_(rec.GetFields());
        if (ret.HasValue()) {
            chosen_values.emplace_back(ret.Value());
            return true;
        }
        return false;
    }

protected:
    Status DeepCopy_Internal(IAsyncContext *&context_copy) {
        return IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }

private:
    std::vector<T> chosen_values;
    parser_t *parser;
    expr_eval eval_;
    std::function<void(int)> check_count;
    uint32_t cnt;
};


TEST(NullDisk, BasicFullScan) {
    // generate some data
    std::experimental::filesystem::remove_all("test");
    std::experimental::filesystem::create_directories("test");
    store_t store{1, 201326592, "test"};
    store.StartSession();

    std::vector<std::string> data;
    data.reserve(NUM_VALUES);
    for (int i = 0; i < NUM_VALUES; ++i)
        data.emplace_back("{\"id\":" + std::to_string(i) + "}");

    for (int i = 0; i < NUM_VALUES; ++i) {
        store.BatchInsert(data[i], 1);
    }
    store.Refresh();

    expr_func<int, adapter_t> func = [](expr_func_args<adapter_t> args) {
        if (!args.empty()) {
            auto value = args[0].GetAsInt();
            if (value.HasValue() && value.Value() % 2 == 0)
                return Nullable<int>(1);
        }

        return Nullable<int>(); // return null
    };

    auto check_func = [](int value) {
        ASSERT_EQ(value, NUM_VALUES / 2);
    };

    auto callback = [](IAsyncContext *ctxt, Status result) {
        ASSERT_EQ(result, Status::Ok);
    };

    AdapterFullScanContext<int, adapter_t> scan_ctx{{"id"}, func, check_func};

    store.FullScan(scan_ctx, callback, 1);
    store.CompletePending(true);

    store.StopSession();
}

TEST(NullDisk, BasicFullScan2) {
    const size_t store_size = 1UL << 31;
    const size_t table_size = 2048;



    std::string BIG_STR(2048, 'X');

    std::vector<std::string> batches;
    batches.reserve(NUM_VALUES);
    for (int i = 0; i < NUM_VALUES; ++i)
        batches.emplace_back(R"({"id":)" + std::to_string(i) + R"(, "garbage": ")" + BIG_STR + "\"}\n");



    std::experimental::filesystem::remove_all("test");
    std::experimental::filesystem::create_directories("test");
    store_t store{table_size, store_size, "test"};
    store.StartSession();

/*    // add PSF
    std::vector<ParserAction> parser_actions;
    PsfLookup psf = store.MakeEzPsf("(Int) actor.id % 2");
    parser_actions.push_back({REGISTER_INLINE_PSF, psf.id});
    store.ApplyParserShift(parser_actions, [](uint64_t safe_address) {});
    store.CompleteAction(true);*/

    for (const auto &batch: batches) {
        store.BatchInsert(batch, 1);
    }

    store.Refresh();


    expr_func<int, adapter_t> func = [](expr_func_args<adapter_t> args) {
        if (!args.empty()) {
            auto value = args[0].GetAsInt();
            if (value.HasValue())
                return Nullable<int>(value.Value() % 2);
        }

        return Nullable<int>(); // return null
    };

    size_t size = batches.size();
    auto check_func = [size](int cnt) { ASSERT_EQ(cnt, size); };

    AdapterFullScanContext<int, adapter_t> scan_context({"id"}, func, check_func);
    auto callback = [](IAsyncContext *ctxt, Status result) {
        ASSERT_EQ(result, Status::Ok);
    };

    store.FullScan(scan_context, callback, 1);
    store.CompletePending(true);


    store.StopSession();
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}