// The main header file for using EzPsf. You should call ezpsf::getPsf<Adapter>(std::string, llvm::orc::LLLazyJit*) to
// create a new function which is returned by the function. Note, this function is not thread safe!
//
// Created by Max Norfolk on 5/29/23.


#pragma once

#include <llvm/ExecutionEngine/Orc/LLJIT.h>

#include "jit/ezpsf_visitor.h"

namespace fishstore::ezpsf {

    typedef bool (*EzPsf)(EzRecord *, void *);


    // information necessary for a PSF
    struct PsfInfo {
        EzPsf psf;
        std::string name;
        fishstore::ezpsf::DataType type;
        std::vector<std::string> fields;
    };

    // The adapter template must support the following:
    // - typename A::field_t (the field type) which contains the NullableMethods defined in
    // datatypes/conversion_declaration.h
    template<typename A>
    void initJit(llvm::orc::LLJIT *jit) {
        // setup symbol table
        llvm::orc::MangleAndInterner mangle(jit->getExecutionSession(), jit->getDataLayout());
        auto builtin_symbols = llvm::orc::absoluteSymbols(ezpsf::type_conversion::getSymbolTable<A>(mangle));
        LLVM_ERROR_CHECK(jit->getMainJITDylib().define(builtin_symbols));
    }

    PsfInfo getPsf(const std::string &str, llvm::orc::LLJIT *jit) {
        class : public antlr4::ANTLRErrorListener {
        public:
            void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
                             size_t charPositionInLine, const std::string &msg, std::exception_ptr e) override {
                std::stringstream ss;
                ss << "Syntax error in position " << charPositionInLine << ". [" << msg << "].";
                LOG(ss.str());
            }

            void reportAmbiguity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
                                 size_t stopIndex,
                                 bool exact, const antlrcpp::BitSet &ambigAlts,
                                 antlr4::atn::ATNConfigSet *configs) override {
                LOG("Ambiguity reported");
            }

            void reportAttemptingFullContext(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
                                             size_t stopIndex, const antlrcpp::BitSet &conflictingAlts,
                                             antlr4::atn::ATNConfigSet *configs) override {
                LOG("Attempting Full Context reported");
            }

            void reportContextSensitivity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
                                          size_t stopIndex, size_t prediction,
                                          antlr4::atn::ATNConfigSet *configs) override {
                LOG("Context Sensitivity reported");
            }
        } error_listener;

        antlr4::ANTLRInputStream input(str);
        ezpsfLexer lexer(&input);
        lexer.addErrorListener(&error_listener);

        antlr4::CommonTokenStream tokens(&lexer);
        ezpsfParser parser(&tokens);

        parser.addErrorListener(&error_listener);
        auto psf = parser.psf();


        ast::EzPsfVisitor visitor;
        auto psf_ast = visitor.createPsf(psf);
        auto func = psf_ast->generate();
        std::string name = func->getName().str();


        // output to a file
        std::error_code ec;
        llvm::raw_fd_ostream module_out(name + ".txt", ec);
        llvm_consts::module->print(module_out, nullptr);


        llvm::orc::ThreadSafeModule ts_module(std::move(llvm_consts::module), std::move(llvm_consts::ctx));
        LLVM_ERROR_CHECK(jit->addIRModule(std::move(ts_module)));

        auto expected_symbol = jit->lookup(name);
        ASSERT(expected_symbol);
        auto psf_func_ptr = (EzPsf) expected_symbol->getAddress();
        auto ret_type = psf_ast->return_type;
        auto fields = psf_ast->fields;
        return {psf_func_ptr, name, ret_type, fields};
    }
}