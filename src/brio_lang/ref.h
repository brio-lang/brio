#ifndef BRIOLANG_SRC_REF_H_
#define BRIOLANG_SRC_REF_H_

#include <brio_lang/tree.h>
#include <brio_lang/symbol.h>


class Ref {
    public:
        Ref(SymbolTable * sym_table);
        void downUp(AST * tree);
        void identifier(AST * node);
    private:
        SymbolTable * table;
        Scope * currentScope;
};


#endif  // BRIOLANG_SRC_REF_H