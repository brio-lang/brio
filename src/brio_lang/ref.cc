///
/// Brio Lang
/// ref.cc
///
/// Resolve symbols from AST
///

#include <brio_lang/ref.h>
#include <brio_lang/exceptions.h>


Ref::Ref(SymbolTable * sym_table){
    table = sym_table;
    currentScope = &sym_table->globals;
};

void Ref::downUp(AST * node){
    // pre-order actions
    if (node->getType() == IdentifierNode::type){
        
        // resolve if not prefixed with "@"
        if(node->parent->children[0]->getNodeText() != Literals::THIS ||
           node->getNodeText() == Literals::THIS){
            this->identifier(node);
        }
    }

    // in-order actions
    for (int i=0; i < node->children.size(); i++){
        AST * child = node->children[i];
        this->downUp(child);
    }
};

void Ref::identifier(AST * id){
    string identifier = id->getNodeText();

    if (identifier == Literals::THIS){
        id->symbol = table->getEnclosingClass(id->scope);
    }
    else if (identifier == Literals::SUPER){
        id->symbol = table->getEnclosingClass(id->scope);
    }
    else if (id->parent->getType() == SuperClassNode::type){
        // set the superClass reference
        AST * descClassId = id->parent->parent->children[0];
        ClassSymbol * descClassSym = dynamic_cast<ClassSymbol*>(table->resolveId(descClassId));
        descClassSym->superClass = dynamic_cast<ClassSymbol*>(table->resolveId(id));
    }
    else if (id->hasAncestor(ImportNode::type)){
        if (id->getNodeText() == id->getSibling(id->parent->getChildCount()-1)->getNodeText()){
            id->symbol = table->resolveId(id);
            if (id->symbol == nullptr){
                throw RuntimeError("unable to resolve symbol '" + identifier + "'", id->getLineNo());
            }
        }
    }
    else if (id->getNodeText() == id->getSibling(0)->getNodeText()){
        // only resolve id->symbol for first part of dot expression
        id->symbol = table->resolveId(id);
        if (id->symbol == nullptr){
            throw RuntimeError("unable to resolve symbol '" + identifier + "'", id->getLineNo());
        }
    }
};