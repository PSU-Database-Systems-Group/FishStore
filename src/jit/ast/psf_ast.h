// AST for PsfFunctions. These will create an LLVM function for each PSF.
//
// Created by Max Norfolk on 4/16/23.


#pragma once

#include <utility>

#include <llvm/IR/Verifier.h>

#include "jit/ast/expr_ast.h"
#include "jit/ast/json_ast.h"
#include "jit/datatypes/conversion.h"

namespace fishstore::ezpsf::ast {
    class PsfFunctionAst {
    public:

        // Create a new PSF function with a given type. default_value may be a nullptr, if the psf
        // has no default value defined. ret is an expression that is evaluated, and may NOT be a nullptr.
        static std::unique_ptr<PsfFunctionAst> create(const std::string &name, ExprAst *default_value, ExprAst *ret) {
            DataType return_type = ret->type();
            ASSERT(ret != nullptr);

            if (default_value != nullptr) {
                ASSERT(default_value->type() == ret->type(),
                       "Default: " + getName(default_value->type()) + ", Ret: " + getName(ret->type()));
            }

            return std::unique_ptr<PsfFunctionAst>(new PsfFunctionAst(name, return_type, default_value, ret));
        }

        // returns true if the return type is a nullable type. This is the same as if there is no default return value
        [[nodiscard]] bool returnIsNullable() const { return default_value == nullptr; }

        [[nodiscard]] llvm::Function *generate() {
            auto llvm_ptr_type = llvm::PointerType::getUnqual(*llvm_consts::ctx);
            llvm::Type *field_param[] = {llvm_ptr_type, llvm_ptr_type};


            // a PSF will have a prototype of:
            // bool myPsf(void* record, (RETURN_TYPE)* ret) {...}
            // if true, then it has a value and ret was changed, otherwise it is false.
            auto ft = llvm::FunctionType::get(llvmConvertType(DataType::BOOL_T), field_param, false);


            auto func = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name, *llvm_consts::module);
            auto *record = func->getArg(0);
            auto *ret_ptr = func->getArg(1);

            record->setName("argRecordPtr");
            ret_ptr->setName("argReturnPtr");

            auto func_body = llvm::BasicBlock::Create(*llvm_consts::ctx, "psf_body", func);

            /////////////////////////////////////////////////////
            // Default Return
            /////////////////////////////////////////////////////


            auto default_ret = llvm::BasicBlock::Create(*llvm_consts::ctx, "default_return", func);
            llvm_consts::builder->SetInsertPoint(default_ret);

            // default return
            if (returnIsNullable()) {
                llvm_consts::builder->CreateRet(LitBoolAst(false).code());
            } else {
                llvm_consts::builder->CreateStore(default_value->code(), ret_ptr);
                llvm_consts::builder->CreateRet(LitBoolAst(true).code());
            }


            llvm_consts::builder->SetInsertPoint(func_body);
            auto fall_through = llvm::BasicBlock::Create(*llvm_consts::ctx, "param_check_fall_through", func);

            /////////////////////////////////////////////////////
            // Convert types
            /////////////////////////////////////////////////////

            // field ids are referenced in the order they are accessed.
            // the actual JSON strings can be accessed by the std::vector<std::string> fields,
            // and the indices into that vector correspond to the expected field_id.
            uint64_t field_id = 0;

            for (const auto &id: JsonRefAst::used_ids) {
                fields.emplace_back(id->json);

                // find the correct field function & extract the value
                DataType field_type = id->type();
                auto get_field_func = type_conversion::getFieldFunc(field_type);
                NULL_CHECK(get_field_func);



                // TODO use LitUInt64 for llvm field id
                llvm::Value *field_id_val = LitInt64Ast(static_cast<int64_t>(field_id++)).code();


                //////////////////////////////////////////////////////////
                // Store nullable field and then extract values from it //
                //////////////////////////////////////////////////////////

                const std::string &prefix = id->json;

                // struct type of the data
                llvm::StructType *field_nullable_type = getNullableType(field_type);
                auto field_alloca = llvm_consts::builder->CreateAlloca(field_nullable_type, nullptr,
                                                                       prefix + "_structAlloca");

                // this call returns void, as field_alloca is updated
                llvm_consts::builder->CreateCall(get_field_func, {record, field_id_val, field_alloca},
                                                 prefix + "_callGetField");


                // get the hasValue entry in the struct
                auto has_gep = llvm_consts::builder->CreateStructGEP(field_nullable_type, field_alloca, 0,
                                                                     prefix + "_hasGep");
                auto has_val = llvm_consts::builder->CreateLoad(llvmConvertType(DataType::BOOL_T), has_gep,
                                                                prefix + "_hasValue");

                // If it does not have a value, goto default_ret
                llvm_consts::builder->CreateCondBr(has_val, fall_through, default_ret);
                llvm_consts::builder->SetInsertPoint(fall_through);

                // get the value entry in the struct
                auto field_gep = llvm_consts::builder->CreateStructGEP(field_nullable_type, field_alloca, 1,
                                                                       prefix + "_fieldGep");
                auto field_val = llvm_consts::builder->CreateLoad(llvmConvertType(field_type), field_gep,
                                                                  prefix + "_fieldValue");

                id->value = field_val;

                fall_through = llvm::BasicBlock::Create(*llvm_consts::ctx, "param_check_fall_through", func);
            }

            llvm_consts::builder->CreateStore(ret->code(), ret_ptr);
            llvm_consts::builder->CreateRet(LitBoolAst(true).code());

            llvm::verifyFunction(*func);
            JsonRefAst::reset();
            return func;
        }


        std::string name;
        DataType return_type;
        ExprAst *default_value;
        ExprAst *ret;

        // this is filled after generate() is called.
        std::vector<std::string> fields;


        virtual ~PsfFunctionAst() {
            delete default_value;
            delete ret;
        }

    private:
        PsfFunctionAst(std::string name, DataType return_type, ExprAst *default_value, ExprAst *ret)
                : name(std::move(name)),
                  return_type(return_type),
                  default_value(default_value),
                  ret(ret) {
        }
    };
}