#ifndef BRIOLANG_SRC_INTERPRETER_H_
#define BRIOLANG_SRC_INTERPRETER_H_

#include <string>
#include <vector>
#include <map>
#include <vector>

#include <brio_lang/tree.h>
#include <brio_lang/symbol.h>
#include <brio_lang/object.h>


using namespace std;


class MemorySpace {
    public:
        MemorySpace(string space_name);
        Object * get(string id);
        void put(string id, Object * value);
        string toString();
        string getName();
        map<string, Object *> members;
    private:
        string name;
};

class MethodSpace : public MemorySpace {
    public:
        MethodSpace(MethodSymbol * method);
        MethodSymbol * def_symbol;
    private:
};

class Stack {
    public:
        Stack();
        MemorySpace * peek();
        int size();
        void push(MethodSpace * space);
        void pop();
        vector<MethodSpace *> methodSpace;
    private:
};

class ReturnValue{
    public:
        ReturnValue();
        Object * value;
    private:
};

class BreakInterrupt{
    public: 
        BreakInterrupt();
    private:
};

class ClassInstance : public MemorySpace, public Object{
    public:
        ClassInstance(ClassSymbol *);
        ClassSymbol * symbol;
        string toString();
        Object * clone();
    private:
};

class ImportedInstance : public MemorySpace, public Object{
    public:
        ImportedInstance(ImportedModuleSymbol *);
        ImportedModuleSymbol * symbol;
        string toString();
        Object * clone();
    private:
};

class Interpreter {
    public:
        Interpreter(ProgramNode * rootNode, SymbolTable * table);
        Interpreter(ProgramNode * rootNode, SymbolTable * table, string path);
        Interpreter(ProgramNode * rootNode, SymbolTable * table, vector<string> args, string path);
        void initialize(ProgramNode * rootNode, SymbolTable * symTable, string path);
        Object * exec();
        Object * exec(AST * node);
        void program(AST * node);
        void block(AST * node);
        void varAssignment(AST * node);
        void varDeclaration(AST * node);
        Object * methodCall(AST * node);
        bool isBuiltInType(Object * obj);
        bool isBuiltInMethodCall(AST * node);
        Object * builtInMethodCall(AST * node);
        Object * builtInTypeMethodCall(Object * obj, string method_id, AST * params);
        Object * builtInTypeAttribute(Object * obj, string attribute);
        Object * main();
        Object * main(vector<Object*> params);
        void classDeclaration(AST * node);
        ClassInstance * classInstantiation(AST * node);
        Object * identifier(AST * node);
        Object * memberAccess(AST * node);
        Object * add(AST * node);
        Object * subtract(AST * node);
        Object * multiply(AST * node);
        Object * divide(AST * node);
        void addAssign(AST * node);
        void subtractAssign(AST * node);
        void multiplyAssign(AST * node);
        void divideAssign(AST * node);
        void paramsList(AST * node);
        void import(AST * node);
        void importedModule(AST * node);
        void execImportedModules(AST * node);
        string getImportRelativePath(AST * node);
        string getProgramPath(AST * node);
        string findImportFile(AST * node);
        Integer * getSize(AST * node);
        Object * parentheses(AST * node);
        Integer * literalInt(AST * node);
        String * literalString(AST * node);
        Decimal * literalDecimal(AST * node);
        Boolean * literalBool(AST * node);
        None * literalNone(AST * node);
        Object * returnStatement(AST * node);
        Array * array(AST * node);
        Dictionary * dictionary(AST * node);
        Boolean * boolAnd(AST * node);
        Boolean * boolOr(AST * node);
        Boolean * boolEquals(AST * node);
        Boolean * boolNotEquals(AST * node);
        Boolean * boolLessThan(AST * node);
        Boolean * boolLessThanOrEqual(AST * node);
        Boolean * boolGreaterThan(AST * node);
        Boolean * boolGreaterThanOrEqual(AST * node);
        Integer * bitShiftLeft(AST * node);
        Integer * bitShiftRight(AST * node);
        Object * modulus(AST * node);
        Object * exponent(AST * node);
        Integer * bitAnd(AST * node);
        Integer * bitOr(AST * node);
        void bitAndAssign(AST * node);
        void bitOrAssign(AST * node);
        Integer * bitXor(AST * node);
        void tryStatement(AST * node);
        void catchStatement(AST * node, string id, string msg, void * e);
        AST * getGenericCatch(AST * node);
        AST * getSpecificCatch(string name, AST * node);
        void raiseStatement(AST * node);
        String * traceback(AST * node);
        void ifStatement(AST * node);
        void whileStatement(AST * node);
        void skipStatement(AST * node);
        void forStatement(AST * node);
        void eachStatement(AST * node);
        void print(AST * node);
        Object * range(AST * node);
        Object * input(AST * node);
        Object * index(AST * node);

        Boolean * boolean(AST * node);
        String * str(AST * node);
        Integer * integer(AST * node);
        Decimal * decimal(AST * node);
        String * arg(AST * node);

        Object * open(AST * node);
        Object * close(AST * node);
        Object * httpGet(AST * node);
        Object * httpPost(AST * node);
        void exit();
        void exit(AST * node);
        Object * load (AST *);
        Object * memberLoad (AST *);
        void memberAssign(AST *, Object *);
        MemorySpace * getSpaceWithSymbol(string id);
        SymbolTable * table;
        ProgramNode * root;
    private:
        string programPath; 
        vector<string> arguments;
        GlobalScope * globalScope;
        MemorySpace * globals;
        MemorySpace * currentSpace;
        Stack stack;
        ReturnValue sharedReturnValue;
};

#endif  // BRIOLANG_SRC_INTERPRETER_H_