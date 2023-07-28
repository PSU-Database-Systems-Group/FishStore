//
//
// Created by Max Norfolk on 7/24/23.


#pragma once

#include <gtest/gtest.h>

#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>

#include "jit/ezpsf.h"
#include "test_adapter.h"

using namespace fishstore::ezpsf;

/**
 * A basic class to setup the JIT stuff necessary for all GTests.
 */
class BasicJitFixture : public ::testing::Test {
protected:
    std::unique_ptr<llvm::orc::LLJIT> jit;

    std::vector<record_t> records;

    std::map<std::string, uint64_t> fields;

    PsfInfo getPsf(const std::string &psf_body) {
        return fishstore::ezpsf::getPsf<TestAdapter>(psf_body, jit.get());
    }

    void SetUp() override {

        /////////////////////////////////////////////////////
        // JIT Stuff
        /////////////////////////////////////////////////////

        // initialize jit stuff
        auto target_triple = llvm::sys::getDefaultTargetTriple();
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmParsers();
        llvm::InitializeAllAsmPrinters();


        auto expected_jit = llvm::orc::LLJITBuilder().create();
        ASSERT_TRUE((bool) expected_jit);

        jit = std::move(expected_jit.get());
    }


    template<typename T>
    void RecBatchInsert(const std::string &key, std::vector<T> &vec) {
        fields[key] = fields.size();
        records.resize(vec.size());
        for (size_t i = 0; i < vec.size(); i++) {
            RecInsert(records[i], key, vec[i]);
        }
    }

    void RecInsert(record_t &rec, const std::string &key, const std::string &value) {
//        auto &ref = strings_buffer.emplace_back(value);
        StringRef field_val = {value.data(), value.size()};
        field_t field(field_val);

        int index = fields[key];
        rec.insert(rec.begin() + index, field);
    }

    void RecInsert(record_t &rec, const std::string &key, int32_t value) {
        field_t field(value);
        int index = fields[key];
        rec.insert(rec.begin() + index, field);
    }
};