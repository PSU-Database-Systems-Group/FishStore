// Used to log errors that occur during the parsing state
//
// Created by Max Norfolk on 2/3/23.


#pragma once

#include <utility>
#include <vector>
#include <string>

class EzPsfException : public std::exception {
public:
    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return cause.c_str();
    }

    explicit EzPsfException(const int line, const std::string &file, const std::string &cause)
            : cause(std::string("[") + file + ":" + std::to_string(line) + "] " + cause + "\n") {}


private:
    const std::string cause;
};


// Some macros used by the JIT stuff to allow the most amount of information if a compilation error occurs
#define OPTIONAL_ARG(DEFAULT, ...) (__VA_OPT__((true) ? __VA_ARGS__ :) DEFAULT)
#define SHORT_FILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


#define LOG(S) macro_wrapper::logError(__LINE__, SHORT_FILE_NAME, S)
#define LOG_RET(S) LOG(S); return nullptr

#define ASSERT(S, ...) if (!(S)) LOG(std::string("Assertion Failed! [" #S "]. ") + OPTIONAL_ARG("", __VA_ARGS__))
#define NULL_CHECK(S) ASSERT(S != nullptr, "Failed null check."); if ((S) == nullptr) return nullptr

#define LLVM_ERROR_CHECK(S) if (auto _error_0xdd0c = S) LOG("Failed error check '" #S "'. Caused by: '" + llvm::toString(std::move(_error_0xdd0c)))

#define VISIT_START try {
#define VISIT_END } catch (const std::exception &ex) { LOG_RET("Exception while parsing line: " + std::to_string(ctx->getStart()->getLine()) + ". Caused by: '" + ex.what() + "'."); } LOG_RET("unreachable");

namespace macro_wrapper {
    EzPsfException logError(const int line, const std::string &file, const std::string &cause) {
        EzPsfException ex(line, file, cause);
        fprintf(stderr, "%s", ex.what());
        return ex;
    }
}