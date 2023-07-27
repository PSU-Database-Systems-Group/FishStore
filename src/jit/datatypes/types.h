// Provides the datatypes supported
//
// Created by Max Norfolk on 5/19/23.


#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include "jit/ast/llvm_consts.h"
#include "jit/ezpsf_exceptions.h"

namespace fishstore::ezpsf {

    // the flags a datatype can be
    enum class DataTypeFlags : uint32_t {
        IS_NUM_TYPE = 1 << 10,
        IS_INT_TYPE = 1 << 11,
        IS_FLOAT_TYPE = 1 << 12,
        UNSIGNED = 1 << 13,
        INLINE_ABLE = 1 << 14, // Can be stored inline in a fishstore record (sizeof(type) <= sizeof(int32_t))
    };

    inline constexpr uint32_t operator|(uint32_t f1, DataTypeFlags f2) {
        return f1 | static_cast<uint32_t>(f2);
    }

    inline constexpr uint32_t operator|(DataTypeFlags f1, DataTypeFlags f2) {
        return static_cast<uint32_t>(f1) | static_cast<uint32_t>(f2);
    }

    // The valid datatypes
    // TODO: INT8_T, unsigned int's, FLOAT_T
    enum class DataType : uint32_t {
        STR_T = 0,

        INT8_T = 10 | DataTypeFlags::IS_NUM_TYPE | DataTypeFlags::INLINE_ABLE,
        INT32_T = 20 | DataTypeFlags::IS_NUM_TYPE | DataTypeFlags::IS_INT_TYPE | DataTypeFlags::INLINE_ABLE,
        INT64_T = 21 | DataTypeFlags::IS_NUM_TYPE | DataTypeFlags::IS_INT_TYPE,

        // unsigned
        UINT32_T = 30 | DataTypeFlags::IS_NUM_TYPE | DataTypeFlags::IS_INT_TYPE
                   | DataTypeFlags::UNSIGNED | DataTypeFlags::INLINE_ABLE,
        UINT64_T = 31 | DataTypeFlags::IS_NUM_TYPE | DataTypeFlags::IS_INT_TYPE | DataTypeFlags::UNSIGNED,

        FLOAT_T = 40 | DataTypeFlags::IS_NUM_TYPE | DataTypeFlags::IS_FLOAT_TYPE | DataTypeFlags::INLINE_ABLE,
        DOUBLE_T = 41 | DataTypeFlags::IS_NUM_TYPE | DataTypeFlags::IS_FLOAT_TYPE,

        BOOL_T = 50 | DataTypeFlags::INLINE_ABLE,

        ERROR_T = 0xffffffff
    };

    inline constexpr bool operator&(DataType type, DataTypeFlags flag) {
        return (static_cast<uint32_t>(type) & static_cast<uint32_t>(flag));
    }

// returns true if the datatype is a number type
    bool isNumType(DataType type) {
        return type & DataTypeFlags::IS_NUM_TYPE;
    }

// returns true if the datatype is an integer type
    bool isIntType(DataType type) {
        return type & DataTypeFlags::IS_INT_TYPE;
    }

// returns true if the datatype is a float type
    bool isFloatType(DataType type) {
        return type & DataTypeFlags::IS_FLOAT_TYPE;
    }

// returns true if the datatype is signed.
    bool isSignedType(DataType type) {
        return !(type & DataTypeFlags::UNSIGNED);
    }

    bool canInline(DataType type) {
        return type & DataTypeFlags::INLINE_ABLE;
    }

// returns the string name of a datatype
    std::string getName(DataType type) {
        switch (type) {
            case DataType::STR_T:
                return "STR_T";
            case DataType::INT8_T:
                return "INT8_T";
            case DataType::INT32_T:
                return "INT32_T";
            case DataType::INT64_T:
                return "INT64_T";
            case DataType::UINT32_T:
                return "UINT32_T";
            case DataType::UINT64_T:
                return "UINT64_T";
            case DataType::DOUBLE_T:
                return "DOUBLE_T";
            case DataType::BOOL_T:
                return "BOOL_T";
            case DataType::ERROR_T:
                return "ERROR_T";
            default:
                LOG("unreachable -- datatypes.h::getName(DataType)");
                return "illegal state";
        }
    }

    std::string getNullableName(DataType type) {
        return "NULLABLE_" + getName(type);
    }

    // converts a DataType to its corresponding LLVM type
    inline llvm::Type *llvmConvertType(DataType type) {
        switch (type) {
            case DataType::STR_T:
                return llvm::StructType::getTypeByName(*llvm_consts::ctx, getName(DataType::STR_T));
            case DataType::INT8_T:
                return llvm::Type::getInt8Ty(*llvm_consts::ctx);
            case DataType::UINT32_T:
            case DataType::INT32_T:
                return llvm::Type::getInt32Ty(*llvm_consts::ctx);
            case DataType::INT64_T:
            case DataType::UINT64_T:
                return llvm::Type::getInt64Ty(*llvm_consts::ctx);
            case DataType::DOUBLE_T:
                return llvm::Type::getDoubleTy(*llvm_consts::ctx);
            case DataType::BOOL_T:
                return llvm::Type::getInt1Ty(*llvm_consts::ctx);
            case DataType::ERROR_T:
                return llvm::Type::getVoidTy(*llvm_consts::ctx);
            default:
            LOG_RET("Illegal DataType in datatypes.h::llvmConvertType");
        }
    }

    // returns a DataType as its Nullable struct type
    inline llvm::StructType *getNullableType(DataType type) {
        auto ret = llvm::StructType::getTypeByName(*llvm_consts::ctx, getNullableName(type));
        NULL_CHECK(ret);
        return ret;
    }

    // creates the nullable struct. This should only be called once per datatype
    inline void createNullableStruct(DataType type) {
        llvm::StructType::create({llvmConvertType(DataType::BOOL_T), llvmConvertType(type)},
                                 getNullableName(type), false);
    }
}