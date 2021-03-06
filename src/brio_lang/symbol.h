#ifndef BRIOLANG_SRC_SYMBOL_H_
#define BRIOLANG_SRC_SYMBOL_H_

#include <string>
#include <map>
#include <brio_lang/tree.h>

using namespace std;


class AST;  // forward reference: tree.h
class Scope;  // forward reference: symbol.h


class Type {
    public:
        virtual string getTypeName() { return "Type"; };
};

class Symbol {
    public:
        Symbol(string name);
        Symbol(string name, Type * type);
        virtual string getSymbolType() { return "Symbol"; };
        string getSymbolName();
        string toString();
        Scope * scope;
        AST * def_node;
    private:
        string name;
        Type * type;
};

class Scope {
    public:
        Scope(Scope * enclosingScope);
        virtual string getScopeName() { return "BaseScope"; };
        virtual Scope * getParentScope();
        Scope * getEnclosingScope();
        void define(Symbol * sym);
        string toString();
        Symbol * resolve(string name);
        virtual Symbol * resolveMember(string name);
        void listMembers();
        map<string, Symbol *> members;
    private:
        Scope * enclosingScope;
};

class GlobalScope : public Scope {
    public:
        GlobalScope();
        virtual string getScopeName() override { return "GlobalScope"; };
    private:
};

class LocalScope : public Scope {
    public:
        LocalScope(Scope * parent);
        virtual string getScopeName() override { return "LocalScope"; };
    private:
};

class BuiltInTypeSymbol : public Symbol, public Type {
    public:
        BuiltInTypeSymbol(string name);
        virtual string getSymbolType() override { return "BuiltInTypeSymbol"; };
    private:
};

class ScopedSymbol : public Symbol, public Scope {
    public:
        ScopedSymbol(string sym_name, Scope * scope);
        virtual string getSymbolType() override { return "ScopedSymbol"; };
        virtual string getScopeName() override { return this->getSymbolName() + "Scope"; };
    private:
};

class ClassSymbol : public ScopedSymbol, public Type {
    public:
        ClassSymbol(string name, Scope * enclosingScope, ClassSymbol * super);
        virtual string getSymbolType() override { return "ClassSymbol"; };
        Scope * getParentScope();
        Symbol * resolveMember(string name);
        ClassSymbol * superClass;
    private:
};

class MethodSymbol : public ScopedSymbol {
    public:
        MethodSymbol(string name, Scope * parent);
        virtual string getSymbolType() override { return "MethodSymbol"; };
    private:
};

class ImportedModuleSymbol : public ScopedSymbol {
    public:
        ImportedModuleSymbol(string name, Scope * parent);
        virtual string getSymbolType() override { return "ImportedModuleSymbol"; };
    private:
};

class BuiltInMethodSymbol : public MethodSymbol {
    public:
        BuiltInMethodSymbol(string name);
        virtual string getSymbolType() override { return "BuiltInMethodSymbol"; };
    private:
};

class VariableSymbol : public Symbol {
    public:
        VariableSymbol(string name);
        VariableSymbol(string name, Type * type);
        virtual string getSymbolType() override { return "VariableSymbol"; };
    private:
};

class SymbolTable {
    public:
        SymbolTable();
        void initTypeSystem();
        Symbol * resolveId(AST * idAST);
        static ClassSymbol * getEnclosingClass(Scope * scope);
        string toString();
        GlobalScope globals;
    private:
};

#endif  // BRIOLANG_SRC_SYMBOL_H_