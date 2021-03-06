#ifndef BRIOLANG_SRC_DEF_H_
#define BRIOLANG_SRC_DEF_H_

#include <brio_lang/tree.h>
#include <brio_lang/symbol.h>


class Def {
    public:
        Def(SymbolTable * sym_table);
        void downUp(AST * tree);
        void enterBlock(AST * node);
        void exitBlock();
        void enterMethod(AST * node);
        void exitMethod();
        void enterClass(AST * node);
        void exitClass();
        void importStatement(AST * node);
        void enterImportedModule(AST * node);
        void exitImportedModule();
        void varDeclaration(AST * node);
        void identifier(AST * node);
    private:
        SymbolTable * table;
        Scope * currentScope;
};


#endif  // BRIOLANG_SRC_DEF_H_