#ifndef BRIOLANG_SRC_VISITOR_DOT_H_
#define BRIOLANG_SRC_VISITOR_DOT_H_


#include <string>
#include <deque>
#include <iostream>
#include <brio_lang/visitor.h>
#include <brio_lang/tree.h>


class DotNode {
    public:
        DotNode(AST * node);
        int num;
        AST * node;
    private:
};


class DotGraphVisitor : TreeVisitor {
    public:
        DotGraphVisitor();
        void visit(AST * node) {};
        string generate(AST * node);
        string getHeader();
        string getBody(AST * node);
        string getFooter();
    private:
        int n_count;
        deque<DotNode*> queue;
};


#endif  // BRIOLANG_SRC_VISITOR_DOT_H_