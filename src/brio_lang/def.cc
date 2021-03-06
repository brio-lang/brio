///
/// Brio Lang
/// def.cc
///
/// Define symbols from AST
///

#include <brio_lang/def.h>
#include <brio_lang/exceptions.h>


Def::Def(SymbolTable * sym_table){
    table = sym_table;
    currentScope = &sym_table->globals;
};

void Def::downUp(AST * node){
    // pre-order actions
    if (node->getType() == MethodDeclarationNode::type){
        this->enterMethod(node);
    } else if (node->getType() == ImportNode::type){
        this->importStatement(node);
    } else if (node->getType() == ImportedModuleNode::type){
        this->enterImportedModule(node);
    } else if (node->getType() == VarDeclarationNode::type){
        this->varDeclaration(node);
    } else if (node->getType() == IdentifierNode::type){
        this->identifier(node);
    } else if (node->getType() == ClassDeclarationNode::type){
        this->enterClass(node);
    } else if (node->getType() == BlockNode::type 
                && node->parent->getType() != ClassDeclarationNode::type
                && node->parent->getType() != MethodDeclarationNode::type
                && node->parent->getType() != ImportedModuleNode::type
                && node->parent->getType() != ImportNode::type
                && node->parent->getType() != FromImportNode::type){
        this->enterBlock(node);
    }

    // in-order actions
    for (int i=0; i < node->children.size(); i++){
        AST * child = node->children[i];
        this->downUp(child);
    }

    // post-order actions
    if (node->getType() == MethodDeclarationNode::type){
        this->exitMethod();
    } else if (node->getType() == ClassDeclarationNode::type){
        this->exitClass();
    } else if (node->getType() == ImportedModuleNode::type){
        this->exitImportedModule();
    } else if (node->getType() == BlockNode::type 
                && node->parent->getType() != ClassDeclarationNode::type
                && node->parent->getType() != MethodDeclarationNode::type
                && node->parent->getType() != ImportedModuleNode::type
                && node->parent->getType() != ImportNode::type
                && node->parent->getType() != FromImportNode::type){
        this->exitBlock();
    }
};

void Def::enterBlock(AST * node){
    currentScope = new LocalScope(currentScope);
};

void Def::exitBlock(){
    currentScope = currentScope->getEnclosingScope();
};

void Def::importStatement(AST * node){
    // define temporary symbol for import
    AST * id = node->children[0];
    string node_id;
    if (id->getType() == IdentifierNode::type){
        node_id = id->getNodeText();
    }
    else if (id->getType() == MemberAccessNode::type){
        node_id = id->children[size(id->children)-1]->getNodeText();
    }else{
        throw RuntimeError("unable to define import statement");
    }
    VariableSymbol * s = new VariableSymbol(node_id);
    s->def_node = id;  // point symbol to AST
    id->symbol = s;  // point AST to symbol
    id->scope = currentScope;  // point AST to scope
    currentScope->define(s);
};

void Def::enterImportedModule(AST * node){
    // define new imported module symbol
    AST * id = node->children[0];
    string node_id = id->getNodeText();
    ImportedModuleSymbol * s = new ImportedModuleSymbol(node_id, currentScope);
    s->def_node = id;  // point symbol to AST
    id->symbol = s;  // point AST to symbol
    id->scope = s;  // point AST to scope
    currentScope->define(s);
    currentScope = s;
};

void Def::exitImportedModule(){
    currentScope = currentScope->getEnclosingScope();
};

void Def::enterMethod(AST * node){
    // define new method symbol
    AST * id = node->children[0];
    string node_id = id->getNodeText();
    MethodSymbol * s = new MethodSymbol(node_id, currentScope);
    s->def_node = id;  // point symbol to AST
    id->symbol = s;  // point AST to symbol
    id->scope = s;  // point AST to scope
    currentScope->define(s);
    currentScope = s;

    // define arguments
    AST * params = node->children[2];
    for (int i=0; i<params->children.size(); i++){
        AST * param = params->children[i];
        if (param->getType() == IdentifierNode::type){

            // define variable symbol for arguments
            VariableSymbol * var_sym = new VariableSymbol(param->getNodeText());
            var_sym->def_node = param;
            param->symbol = var_sym;
            param->scope = currentScope;
            currentScope->define(var_sym);
        }else{
            throw ValueError("method param list must be ids", param->getLineNo());
        }
    }
};

void Def::exitMethod(){
    currentScope = currentScope->getEnclosingScope();
};

void Def::enterClass(AST * node){
    // lookup super class to set scope
    AST * super = node->children[2];
    if (super->children.size() > 0){
        AST * superId = super->children[0];
        superId->scope = currentScope;
    }

    // define new class symbol  (set superclass in ref phase)
    AST * id = node->children[0];
    string node_id = id->getNodeText();
    ClassSymbol * s = new ClassSymbol(node_id, currentScope, nullptr);
    s->def_node = id;  // point symbol to AST
    id->symbol = s;  // point AST to symbol
    id->scope = s;  // point AST to scope
    currentScope->define(s);
    currentScope = s;
};

void Def::exitClass(){
    currentScope = currentScope->getEnclosingScope();
};

void Def::identifier(AST * id){
    id->scope = currentScope;

    if(id->hasAncestor(AsNode::type) && id->getNodeText() == id->parent->children[id->parent->children.size()-1]->getNodeText()){
        string node_id = id->getNodeText();
        VariableSymbol * s = new VariableSymbol(node_id);
        s->def_node = id;  // point symbol to AST
        id->symbol = s;  // point AST to symbol
        id->scope = currentScope;  // point AST to scope
        currentScope->define(s);
    }
};

void Def::varDeclaration(AST * node){
    // define new variable symbol
    AST * id = node->children[0];
    AST * declaration = node->children[1];
    string node_id = id->getNodeText();
    VariableSymbol * s = new VariableSymbol(node_id);
    s->def_node = id;  // point symbol to AST
    id->symbol = s;  // point AST to symbol
    id->scope = currentScope;  // point AST to scope
    currentScope->define(s);
};