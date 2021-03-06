///
/// Brio Lang
/// symbol.cc
///
/// Nested Scope Symbol Table
///

#include <brio_lang/symbol.h>
#include <brio_lang/tree.h>
#include <brio_lang/exceptions.h>

#include <string>
#include <iostream>


Symbol::Symbol(string symbol_name){
    name = symbol_name;
};

Symbol::Symbol(string symbol_name, Type * symbol_type){
    name = symbol_name;
    type = symbol_type;
};

string Symbol::toString(){
    return "<'" + this->getSymbolName() + "'>";
};

string Symbol::getSymbolName(){
    return name;
};

ScopedSymbol::ScopedSymbol(string sym_name, Scope * scope):
    Symbol(sym_name), Scope(scope) { };

BuiltInTypeSymbol::BuiltInTypeSymbol(string sym_name) :
    Symbol(sym_name) { };

BuiltInMethodSymbol::BuiltInMethodSymbol(string name) :
    MethodSymbol(name, NULL) { };

ClassSymbol::ClassSymbol(string sym_name, Scope * enclosingScope, ClassSymbol * super):
    ScopedSymbol(sym_name, enclosingScope) { 
        superClass = super;
    };

Scope * ClassSymbol::getParentScope(){
    if (this->superClass == nullptr){
        return getEnclosingScope();
    }
    return this->superClass;
};

Symbol * ClassSymbol::resolveMember(string name){
    Symbol * s = this->members[name];
    if (s != nullptr) return s;

    // check super class chain only
    if (superClass != nullptr){
        return superClass->resolveMember(name);
    }

    return nullptr;
};

ImportedModuleSymbol::ImportedModuleSymbol(string sym_name, Scope * parent):
    ScopedSymbol(sym_name, parent) { };

MethodSymbol::MethodSymbol(string sym_name, Scope * parent):
    ScopedSymbol(sym_name, parent) { };

VariableSymbol::VariableSymbol(string sym_name) :
    Symbol(sym_name, nullptr) { };

VariableSymbol::VariableSymbol(string sym_name, Type * sym_type) :
    Symbol(sym_name, sym_type) { };

Scope::Scope(Scope * parent){
    this->enclosingScope = parent;
};

Scope * Scope::getEnclosingScope(){
    return this->enclosingScope;
};

Scope * Scope::getParentScope(){
    return this->getEnclosingScope();
}

void Scope::listMembers(){
    std::cout << "listMembers for: " << this->getScopeName() << std::endl;
    for(map<string,Symbol*>::iterator it = members.begin(); it != members.end(); ++it) {
        std::cout << "--" + it->first << std::endl;
    }
}

void Scope::define(Symbol * sym){
    this->members[sym->getSymbolName()] = sym;
    sym->scope = this;
};

Symbol * Scope::resolve(string name){
    Symbol * s = this->members[name];
    if (s != nullptr) return s;

    if (this->getParentScope() != nullptr){
        return this->getParentScope()->resolve(name);
    }

    return nullptr;
};

Symbol * Scope::resolveMember(string name){
    return this->members[name];
};

string Scope::toString(){
    string output = this->getScopeName() + "\n";
    for(map<string,Symbol*>::iterator it = members.begin(); it != members.end(); ++it) {
        string name = it->first;
        output.append("<'" + name + "'> ");
    }
    output += "\n";
    return output;
};

GlobalScope::GlobalScope() : 
    Scope(nullptr) { };

LocalScope::LocalScope(Scope * parent) : 
    Scope(parent) { };

SymbolTable::SymbolTable(){
    this->initTypeSystem();
};

void SymbolTable::initTypeSystem(){
    // init built-in type symbols
    this->globals.define(new BuiltInTypeSymbol("Integer"));
    this->globals.define(new BuiltInTypeSymbol("Decimal"));
    this->globals.define(new BuiltInTypeSymbol("String"));
    this->globals.define(new BuiltInTypeSymbol("Boolean"));
    this->globals.define(new BuiltInTypeSymbol("Array"));
    this->globals.define(new BuiltInTypeSymbol("Dictionary"));
    this->globals.define(new BuiltInTypeSymbol("None"));
    this->globals.define(new BuiltInTypeSymbol("File"));
    this->globals.define(new BuiltInTypeSymbol("Socket"));

    // init built-in exception symbols
    this->globals.define(new BuiltInTypeSymbol("BaseException"));
    this->globals.define(new BuiltInTypeSymbol("ClassException"));
    this->globals.define(new BuiltInTypeSymbol("Exception"));
    this->globals.define(new BuiltInTypeSymbol("LexerError"));
    this->globals.define(new BuiltInTypeSymbol("ParserError"));
    this->globals.define(new BuiltInTypeSymbol("RecognitionException"));
    this->globals.define(new BuiltInTypeSymbol("MismatchedTokenException"));
    this->globals.define(new BuiltInTypeSymbol("SyntaxError"));
    this->globals.define(new BuiltInTypeSymbol("ArithmeticError"));
    this->globals.define(new BuiltInTypeSymbol("BufferError"));
    this->globals.define(new BuiltInTypeSymbol("IOError"));
    this->globals.define(new BuiltInTypeSymbol("LookupError"));
    this->globals.define(new BuiltInTypeSymbol("AttributeError"));
    this->globals.define(new BuiltInTypeSymbol("EOFError"));
    this->globals.define(new BuiltInTypeSymbol("ImportError"));
    this->globals.define(new BuiltInTypeSymbol("ModuleNotFoundError"));
    this->globals.define(new BuiltInTypeSymbol("IndexError"));
    this->globals.define(new BuiltInTypeSymbol("KeyError"));
    this->globals.define(new BuiltInTypeSymbol("KeyboardInterrupt"));
    this->globals.define(new BuiltInTypeSymbol("MemoryError"));
    this->globals.define(new BuiltInTypeSymbol("NameError"));
    this->globals.define(new BuiltInTypeSymbol("NotImplementedError"));
    this->globals.define(new BuiltInTypeSymbol("OSError"));
    this->globals.define(new BuiltInTypeSymbol("OverflowError"));
    this->globals.define(new BuiltInTypeSymbol("RecursionError"));
    this->globals.define(new BuiltInTypeSymbol("RuntimeError"));
    this->globals.define(new BuiltInTypeSymbol("SystemError"));
    this->globals.define(new BuiltInTypeSymbol("TypeError"));
    this->globals.define(new BuiltInTypeSymbol("ValueError"));
    this->globals.define(new BuiltInTypeSymbol("ZeroDivisionError"));
    this->globals.define(new BuiltInTypeSymbol("FileExistsError"));
    this->globals.define(new BuiltInTypeSymbol("FileNotFoundError"));
    this->globals.define(new BuiltInTypeSymbol("TimeoutError"));
    
    // init built-in method symbols
    this->globals.define(new BuiltInMethodSymbol("print"));
    this->globals.define(new BuiltInMethodSymbol("range"));
    this->globals.define(new BuiltInMethodSymbol("traceback"));
    this->globals.define(new BuiltInMethodSymbol("input"));
    this->globals.define(new BuiltInMethodSymbol("size"));
    this->globals.define(new BuiltInMethodSymbol("open"));
    this->globals.define(new BuiltInMethodSymbol("close"));
    this->globals.define(new BuiltInMethodSymbol("exit"));
    this->globals.define(new BuiltInMethodSymbol("boolean"));
    this->globals.define(new BuiltInMethodSymbol("string"));
    this->globals.define(new BuiltInMethodSymbol("integer"));
    this->globals.define(new BuiltInMethodSymbol("decimal"));
    this->globals.define(new BuiltInMethodSymbol("arg"));

    // init temp built-in method symbols (will go away)
    this->globals.define(new BuiltInMethodSymbol("httpGet"));
    this->globals.define(new BuiltInMethodSymbol("httpPost"));
};

string SymbolTable::toString(){
    return this->globals.toString();
};

Symbol * SymbolTable::resolveId(AST * idAST){
    string id = idAST->getNodeText();
    if (idAST->scope == nullptr){
        throw RuntimeError("'" + idAST->getNodeText() + "' does not have a scope defined", idAST->getLineNo());
    }
    
    Symbol * s = idAST->scope->resolve(id);
    if (s == nullptr){
        throw RuntimeError("unable to resolve symbol '" + idAST->getNodeText() + "'", idAST->getLineNo());
    }

    // return symbol if built-in type or method
    if (dynamic_cast<BuiltInMethodSymbol*>(s) != nullptr ||
        dynamic_cast<BuiltInTypeSymbol*>(s) != nullptr) return s;

    // if symbol has no definition node, throw runtime error
    if (s->def_node == nullptr){
        throw RuntimeError("unable to resolve symbol AST '" + idAST->getNodeText() + "'", idAST->getLineNo());
    }

    // if resolves to local or global symbol, token index of
    // definition must be before token index or reference
    int id_location = idAST->getTokenIndex();
    int def_location = s->def_node->getTokenIndex();

    return s;
};

ClassSymbol * SymbolTable::getEnclosingClass(Scope * scope){
    // 'this' and 'super' need to know about enclosing class
    while (scope != nullptr){
        if (dynamic_cast<ClassSymbol*>(scope) != nullptr) {
            return dynamic_cast<ClassSymbol*>(scope);
        }
        scope = scope->getParentScope();
    };
    return nullptr;
};