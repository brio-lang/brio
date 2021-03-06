#ifndef BRIOLANG_SRC_VISITOR_H_
#define BRIOLANG_SRC_VISITOR_H_

#include <brio_lang/tree.h>
#include <iostream>
#include <string>
#include <vector>


class TreeVisitor {
    public:
        virtual void visit(AST * node) { };
    private:
};

class IndependentPrintVisitor : public TreeVisitor {
    public:
        void visit(AST * node);
        void visit(AST * node, int depth);
    private:
};

class InvokeMainMethodPrint : TreeVisitor {
    public:
        void visit(ProgramNode * node);
    private:
};

class OperatorPrecedenceVisitor : TreeVisitor {
    public:
        void visit(AST * node);
    private:
};

#endif  // BRIOLANG_SRC_VISITOR_H_