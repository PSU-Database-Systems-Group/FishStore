//
//
// Created by Max Norfolk on 9/29/23.


#pragma once

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

        auto raw_text = rec.GetRawText();

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

    inline void Finalize() {
        printf("%u record has been touched...\n", cnt);
    }

    inline fishstore::core::KeyHash get_hash() const {
        return fishstore::core::KeyHash{fishstore::core::Utility::GetHashCode(psf_id_, value_)};
    }

    inline bool check(const fishstore::core::KeyPointer *kpt) {
        return kpt->mode == 1 && kpt->inline_psf_id == psf_id_ && kpt->value == value_;
    }

protected:
    Status DeepCopy_Internal(IAsyncContext *&context_copy) {
        return IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }

private:
    uint32_t psf_id_;
    int32_t value_;
    uint32_t cnt;
};