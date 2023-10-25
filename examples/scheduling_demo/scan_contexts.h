//
//
// Created by Max Norfolk on 9/29/23.


#pragma once

#include "adapters/simdjson_adapter.h"
#include "core/fishstore.h"

/////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////

template<typename T>
using Nullable = fishstore::adapter::Nullable<T>;

typedef fishstore::adapter::StringRef StringRef;

// A function similar to an EzPsf, but it will be run after the data has already
// been ingested during a Scan
template<typename A>
using expr_func_args = std::vector<typename A::field_t>;

template<typename T, typename A>
using expr_func = std::function<Nullable<T>(expr_func_args<A>)>;


/////////////////////////////////////////////////////
// Contexts
/////////////////////////////////////////////////////


template<typename T, typename A>
class AdapterFullScanContext : public IAsyncContext {
    typedef typename A::parser_t parser_t;
    typedef typename A::record_t record_t;
    typedef typename A::field_t field_t;

    typedef expr_func<T, A> expr_eval;
public:
    AdapterFullScanContext() = delete;

    ~AdapterFullScanContext() override { delete parser; }

    AdapterFullScanContext(const std::vector<std::string> &field_names, expr_eval eval)
            : parser(A::NewParser(field_names)),
              eval_(eval),
              cnt(0) {
    }

    inline void Touch(const char *payload, uint32_t payload_size) {
        // printf("Record Hit: %.*s\n", payload_size, payload);
        ++cnt;
    }

    inline void Finalize() {
        printf("%u record has been touched...\n", cnt);
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
    Status DeepCopy_Internal(IAsyncContext *&context_copy) override {
        return IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }

private:
    std::vector<T> chosen_values;
    parser_t *parser;
    expr_eval eval_;
    uint32_t cnt;
};


class JsonInlineScanContext : public IAsyncContext {
public:
    JsonInlineScanContext(uint32_t psf_id, int32_t value)
            : psf_id_(psf_id), value_(value), cnt(0) {}

    inline void Touch(const char *payload, uint32_t payload_size) {
        // printf("Record Hit: %.*s\n", payload_size, payload);
        ++cnt;
    }

    inline void Finalize() const {
        printf("%u record has been touched...\n", cnt);
    }

    [[nodiscard]] inline fishstore::core::KeyHash get_hash() const {
        return fishstore::core::KeyHash{fishstore::core::Utility::GetHashCode(psf_id_, value_)};
    }

    inline bool check(const fishstore::core::KeyPointer *kpt) const {
        return kpt->mode == 1 && kpt->inline_psf_id == psf_id_ && kpt->value == value_;
    }

protected:
    Status DeepCopy_Internal(IAsyncContext *&context_copy) override {
        return IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }

private:
    uint32_t psf_id_;
    int32_t value_;
    uint32_t cnt;
};


// context used for joins
template<typename A>
class BadInnerJoinContext : public IAsyncContext {
    typedef typename A::parser_t parser_t;
    typedef typename A::record_t record_t;
    typedef typename A::field_t field_t;

public:
    BadInnerJoinContext() = delete;

    ~BadInnerJoinContext() override { delete parser; }

    BadInnerJoinContext(const std::string &join_key, int id)
            : parser(A::NewParser({join_key})),
              id(id) {
    }

    inline void Touch(const char *payload, uint32_t payload_size) {
        records.emplace_back(payload, payload_size);
    }

    inline void Finalize() {
        assert(records.size() == 1);
        //printf("%u record for join id: %d\n", records.size(), id);
    }

    inline bool check(const char *payload, uint32_t payload_size) {
        parser->Load(payload, payload_size);

        // check to make sure the parser has a value
        bool check = parser->HasNext();
        assert(check);

        record_t rec = parser->NextRecord();
        assert(!parser->HasNext());


        // check if id's match
        return (rec.GetFields()[0].GetAsInt().Value() == id);
    }

protected:
    Status DeepCopy_Internal(IAsyncContext *&context_copy) override {
        return IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }

private:
    std::vector<StringRef> records;
    parser_t *parser;
    int id;
};

// slow join without any special stuff
template<typename D, typename A, typename InnerContext>
class FullScanAdapterJoinContext : public IAsyncContext {
    typedef typename A::parser_t parser_t;
    typedef typename A::record_t record_t;
    typedef typename A::field_t field_t;

    typedef fishstore::core::FishStore<D, A> store_t;

public:
    FullScanAdapterJoinContext() = delete;

    ~FullScanAdapterJoinContext() override { delete parser; }

    FullScanAdapterJoinContext(const std::vector<std::string> &field_names, store_t *inner)
            : parser(A::NewParser(field_names)),
              inner(inner),
              cnt(0) {

    }

    inline void Touch(const char *payload, uint32_t payload_size) {
        ++cnt;
    }

    inline void Finalize() {
        printf("%u record has been touched...\n", cnt);
    }

    inline bool check(const char *payload, uint32_t payload_size) {
        parser->Load(payload, payload_size);

        // check to make sure the parser has a value
        bool check = parser->HasNext();
        assert(check);

        record_t rec = parser->NextRecord();
        assert(!parser->HasNext());


        // evaluate
        Nullable<int> id = rec.GetFields()[0].GetAsInt();

        auto callback = [](IAsyncContext *ctxt, Status result) {
            assert(result == Status::Ok);
        };


        if (id.HasValue()) {
            int non_null_id = id.Value();

            InnerContext scan_ctx("id", non_null_id);
            inner->FullScan(scan_ctx, callback, 1);
            return true;
        }
        return false;
    }

protected:
    Status DeepCopy_Internal(IAsyncContext *&context_copy) override {
        return IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }

private:
    parser_t *parser;
    store_t *inner;
    uint32_t cnt;
};


class HashInnerJoinContext : public IAsyncContext {
public:
    HashInnerJoinContext() = delete;

    ~HashInnerJoinContext() override = default;

    HashInnerJoinContext(uint32_t psf_id, int id)
            : psf_id_(psf_id),
              join_key_id(id) {
    }

    inline void Touch(const char *payload, uint32_t payload_size) {
        records.emplace_back(payload, payload_size);
    }

    inline void Finalize() {
        assert(records.size() == 1);
    }

    [[nodiscard]] inline fishstore::core::KeyHash get_hash() const {
        return fishstore::core::KeyHash{fishstore::core::Utility::GetHashCode(psf_id_, join_key_id)};
    }

    inline bool check(const fishstore::core::KeyPointer *kpt) const {
        return kpt->mode == 1 && kpt->inline_psf_id == psf_id_ && kpt->value == join_key_id;
    }

protected:
    Status DeepCopy_Internal(IAsyncContext *&context_copy) override {
        return IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }

private:
    std::vector<StringRef> records;
    int join_key_id;
    uint32_t psf_id_;
};

template<typename D, typename A, typename InnerContext>
class ScanAdapterJoinContext : public IAsyncContext {
    typedef typename A::parser_t parser_t;
    typedef typename A::record_t record_t;
    typedef typename A::field_t field_t;

    typedef fishstore::core::FishStore<D, A> store_t;

public:
    ScanAdapterJoinContext() = delete;

    ~ScanAdapterJoinContext() override { delete parser; }

    ScanAdapterJoinContext(const std::vector<std::string> &field_names, store_t *inner, uint32_t inner_psf_id)
            : parser(A::NewParser(field_names)),
              inner(inner),
              inner_psf_id(inner_psf_id),
              cnt(0) {

    }

    inline void Touch(const char *payload, uint32_t payload_size) {
        ++cnt;
    }

    inline void Finalize() {
        printf("%u record has been touched...\n", cnt);
    }

    inline bool check(const char *payload, uint32_t payload_size) {
        parser->Load(payload, payload_size);

        // check to make sure the parser has a value
        bool check = parser->HasNext();
        assert(check);

        record_t rec = parser->NextRecord();
        assert(!parser->HasNext());


        // evaluate
        Nullable<int> id = rec.GetFields()[0].GetAsInt();

        auto callback = [](IAsyncContext *ctxt, Status result) {
            assert(result == Status::Ok);
        };


        if (id.HasValue()) {
            int non_null_id = id.Value();

            InnerContext scan_ctx(inner_psf_id, non_null_id);
            inner->Scan(scan_ctx, callback, 1);
            return true;
        }
        return false;
    }

protected:
    Status DeepCopy_Internal(IAsyncContext *&context_copy) override {
        return IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }

private:
    parser_t *parser;
    store_t *inner;
    uint32_t inner_psf_id;
    uint32_t cnt;
};