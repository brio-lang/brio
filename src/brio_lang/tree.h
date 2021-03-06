#ifndef BRIOLANG_SRC_TREE_H_
#define BRIOLANG_SRC_TREE_H_

#include <brio_lang/token.h>
#include <brio_lang/symbol.h>

#include <vector>
#include <string>

using namespace std;


class Scope;  // forward reference: symbol.h
class Symbol;  // forward reference: symbol.h
class Type;  // forward reference: symbol.h


class AST {
    public:
        AST();
        AST(Token node_token);
        virtual string getType() { return "AST"; };
        void addChild(AST * node);
        AST * popChild();
        void replace(AST * node);
        int getChildCount();
        Token getToken();
        int getTokenType();
        int getTokenIndex();
        string getNodeText();
        int getBindingPower();
        int getLineNo();
        bool hasAncestorExpr();
        bool hasAncestor(string node_type);
        AST * getAncestor(string node_type);
        AST * getSibling(int index);
        bool isNil();
        virtual AST * clone();
        AST * parent;
        vector<AST*> children;
        string type;
        Scope * scope;  // set by Def.cc
        Symbol * symbol;  // set by Ref.cc
        Type * evalType;  // set by Ref.cc
    private:
        int index;
        Token token;
};

class ProgramNode : public AST {  
    public:
        ProgramNode();
        inline static const string type = "ProgramNode";
        virtual string getType() override { return this->type; };
        virtual ProgramNode * clone() override { return new ProgramNode(*this); };
    private:
};

class VarDeclarationNode : public AST {
    public:
        VarDeclarationNode(Token token);
        inline static const string type = "VarDeclarationNode";
        virtual string getType() override { return this->type; }; 
        virtual VarDeclarationNode * clone() override { return new VarDeclarationNode(*this); }; 
    private:
};

class VarAssignmentNode : public AST {
    public:
        VarAssignmentNode(Token token);
        inline static const string type = "VarAssignmentNode";
        virtual string getType() override { return this->type; };  
        virtual VarAssignmentNode * clone() override { return new VarAssignmentNode(*this); };
    private:
};

class AdditionAssignNode : public AST {
    public:
        AdditionAssignNode(Token token);
        inline static const string type = "AdditionAssignNode";
        virtual string getType() override { return this->type; };  
        virtual AdditionAssignNode * clone() override { return new AdditionAssignNode(*this); };
    private:
};

class SubtractionAssignNode : public AST {
    public:
        SubtractionAssignNode(Token token);
        inline static const string type = "SubtractionAssignNode";
        virtual string getType() override { return this->type; };  
        virtual SubtractionAssignNode * clone() override { return new SubtractionAssignNode(*this); };
    private:
};

class MultiplicationAssignNode : public AST {
    public:
        MultiplicationAssignNode(Token token);
        inline static const string type = "MultiplicationAssignNode";
        virtual string getType() override { return this->type; };  
        virtual MultiplicationAssignNode * clone() override { return new MultiplicationAssignNode(*this); };
    private:
};

class DivisionAssignNode : public AST {
    public:
        DivisionAssignNode(Token token);
        inline static const string type = "DivisionAssignNode";
        virtual string getType() override { return this->type; };  
        virtual DivisionAssignNode * clone() override { return new DivisionAssignNode(*this); };
    private:
};

class BitOrAssignNode : public AST {
    public:
        BitOrAssignNode(Token token);
        inline static const string type = "BitOrAssignNode";
        virtual string getType() override { return this->type; };  
        virtual BitOrAssignNode * clone() override { return new BitOrAssignNode(*this); };
    private:
};

class BitAndAssignNode : public AST {
    public:
        BitAndAssignNode(Token token);
        inline static const string type = "BitAndAssignNode";
        virtual string getType() override { return this->type; };  
        virtual BitAndAssignNode * clone() override { return new BitAndAssignNode(*this); };
    private:
};

class ModifierNode: public AST {
    public:
        ModifierNode(Token token);
        inline static const string type = "ModifierNode";
        virtual string getType() override { return this->type; };
        virtual ModifierNode * clone() override { return new ModifierNode(*this); };
    private:
};

class MethodDeclarationNode : public AST {
    public:
        MethodDeclarationNode(Token token);
        inline static const string type = "MethodDeclarationNode";
        virtual string getType() override { return this->type; };  
        virtual MethodDeclarationNode * clone() override { return new MethodDeclarationNode(*this); };
    private:
};

class MethodCallNode : public AST {
    public:
        MethodCallNode(Token token);
        inline static const string type = "MethodCallNode";
        virtual string getType() override { return this->type; };  
        virtual MethodCallNode * clone() override { return new MethodCallNode(*this); };
    private:
};

class ClassDeclarationNode : public AST {
    public:
        ClassDeclarationNode(Token token);
        inline static const string type = "ClassDeclarationNode";
        virtual string getType() override { return this->type; };  
        virtual ClassDeclarationNode * clone() override { return new ClassDeclarationNode(*this); };
    private:
};

class ClassInstantiationNode : public AST {
    public:
        ClassInstantiationNode(Token token);
        inline static const string type = "ClassInstantiationNode";
        virtual string getType() override { return this->type; };  
        virtual ClassInstantiationNode * clone() override { return new ClassInstantiationNode(*this); };
    private:
};

class BlockNode : public AST {
    public:
        BlockNode(Token token);
        inline static const string type = "BlockNode";
        virtual string getType() override { return this->type; };  
        virtual BlockNode * clone() override { return new BlockNode(*this); };
    private:
};

class BinExpressionNode : public AST {
    public:
        BinExpressionNode(Token token);
        inline static const string type = "BinExpressionNode";
        virtual string getType() override { return this->type; };  
        virtual BinExpressionNode * clone() override { return new BinExpressionNode(*this); };
    private:
};

class BoolExpressionNode : public AST {
    public:
        BoolExpressionNode(Token token);
        inline static const string type = "BoolExpressionNode";
        virtual string getType() override { return this->type; };  
        virtual BoolExpressionNode * clone() override { return new BoolExpressionNode(*this); };
    private:
};

class BitExpressionNode : public AST {
    public:
        BitExpressionNode(Token token);
        inline static const string type = "BitExpressionNode";
        virtual string getType() override { return this->type; };  
        virtual BitExpressionNode * clone() override { return new BitExpressionNode(*this); };
    private:
};

class AddExpressionNode : public BinExpressionNode {
    public:
        AddExpressionNode(Token token);
        inline static const string type = "AddExpressionNode";
        virtual string getType() override { return this->type; };  
        virtual AddExpressionNode * clone() override { return new AddExpressionNode(*this); };
    private:
};

class SubtractExpressionNode : public BinExpressionNode {
    public:
        SubtractExpressionNode(Token token);
        inline static const string type = "SubtractExpressionNode";
        virtual string getType() override { return this->type; };
        virtual SubtractExpressionNode * clone() override { return new SubtractExpressionNode(*this); };
    private:
};

class MultiplyExpressionNode : public BinExpressionNode {
    public:
        MultiplyExpressionNode(Token token);
        inline static const string type = "MultiplyExpressionNode";
        virtual string getType() override { return this->type; };  
        virtual MultiplyExpressionNode * clone() override { return new MultiplyExpressionNode(*this); };
    private:
};

class DivideExpressionNode : public BinExpressionNode {
    public:
        DivideExpressionNode(Token token);
        inline static const string type = "DivideExpressionNode";
        virtual string getType() override { return this->type; }; 
        virtual DivideExpressionNode * clone() override { return new DivideExpressionNode(*this); }; 
    private:
};

class MemberAccessNode : public AST {
    public:
        MemberAccessNode(Token token);
        inline static const string type = "MemberAccessNode";
        virtual string getType() override { return this->type; };
        virtual MemberAccessNode * clone() override { return new MemberAccessNode(*this); };
    private:
};

class IndexNode : public AST {
    public:
        IndexNode(Token token);
        inline static const string type = "IndexNode";
        virtual string getType() override { return this->type; };
        virtual IndexNode * clone() override { return new IndexNode(*this); };
    private:
};

class ParamsListNode : public AST { 
    public:
        ParamsListNode(Token token);
        inline static const string type = "ParamsListNode";
        virtual string getType() override { return this->type; };  
        virtual ParamsListNode * clone() override { return new ParamsListNode(*this); };
    private:
};

class ParenthesesNode : public AST { 
    public:
        ParenthesesNode(Token token);
        inline static const string type = "ParenthesesNode";
        virtual string getType() override { return this->type; };  
        virtual ParenthesesNode * clone() override { return new ParenthesesNode(*this); };
    private:
};

class SuperClassNode : public AST {
    public:
        SuperClassNode(Token token);
        inline static const string type = "SuperClassNode";
        virtual string getType() override { return this->type; };  
        virtual SuperClassNode * clone() override { return new SuperClassNode(*this); };
    private:
};

class LiteralIntNode : public AST {
    public:
        LiteralIntNode(Token token);
        inline static const string type = "LiteralIntNode";
        virtual string getType() override { return this->type; };  
        virtual LiteralIntNode * clone() override { return new LiteralIntNode(*this); };
    private:
};

class LiteralStringNode : public AST {
    public:
        LiteralStringNode(Token token);
        inline static const string type = "LiteralStringNode";
        virtual string getType() override { return this->type; };  
        virtual LiteralStringNode * clone() override { return new LiteralStringNode(*this); };
    private:
};

class LiteralDecimalNode : public AST {
    public:
        LiteralDecimalNode(Token token);
        inline static const string type = "LiteralDecimalNode";
        virtual string getType() override { return this->type; };
        virtual LiteralDecimalNode * clone() override { return new LiteralDecimalNode(*this); };
    private:
};

class LiteralBooleanNode : public AST {
    public:
        LiteralBooleanNode(Token token);
        inline static const string type = "LiteralBooleanNode";
        virtual string getType() override { return this->type; };
        virtual LiteralBooleanNode * clone() override { return new LiteralBooleanNode(*this); };
    private:
};

class LiteralNoneNode : public AST {
    public:
        LiteralNoneNode(Token token);
        inline static const string type = "LiteralNoneNode";
        virtual string getType() override { return this->type; };
        virtual LiteralNoneNode * clone() override { return new LiteralNoneNode(*this); };
    private:
};

class ArrayNode : public AST {
    public:
        ArrayNode(Token token);
        inline static const string type = "ArrayNode";
        virtual string getType() override { return this->type; };
        virtual ArrayNode * clone() override { return new ArrayNode(*this); };
    private:
};

class DictionaryNode : public AST {
    public:
        DictionaryNode(Token token);
        inline static const string type = "DictionaryNode";
        virtual string getType() override { return this->type; };
        virtual DictionaryNode * clone() override { return new DictionaryNode(*this); };
    private:
};
class DictionaryKeyValueNode : public AST {
    public:
        DictionaryKeyValueNode(Token token);
        inline static const string type = "DictionaryKeyValueNode";
        virtual string getType() override { return this->type; };
        virtual DictionaryKeyValueNode * clone() override { return new DictionaryKeyValueNode(*this); };
    private:
};

class ReturnNode : public AST {
    public:
        ReturnNode(Token token);
        inline static const string type = "ReturnNode";
        virtual string getType() override { return this->type; };  
        virtual ReturnNode * clone() override { return new ReturnNode(*this); };
    private:
};

class IdentifierNode : public AST {
    public:
        IdentifierNode(Token token);
        inline static const string type = "IdentifierNode";
        virtual string getType() override { return this->type; };
        virtual IdentifierNode * clone() override { return new IdentifierNode(*this); };
    private:
};

class BoolAndNode : public BoolExpressionNode {
    public:
        BoolAndNode(Token token);
        inline static const string type = "BoolAndNode";
        virtual string getType() override { return this->type; };
        virtual BoolAndNode * clone() override { return new BoolAndNode(*this); };
    private:
};

class BoolOrNode : public BoolExpressionNode {
    public:
        BoolOrNode(Token token);
        inline static const string type = "BoolOrNode";
        virtual string getType() override { return this->type; };
        virtual BoolOrNode * clone() override { return new BoolOrNode(*this); };
    private:
};

class BoolEqualsNode : public BoolExpressionNode {
    public:
        BoolEqualsNode(Token token);
        inline static const string type = "BoolEqualsNode";
        virtual string getType() override { return this->type; };
        virtual BoolEqualsNode * clone() override { return new BoolEqualsNode(*this); };
    private:
};

class BoolNotEqualsNode : public BoolExpressionNode {
    public:
        BoolNotEqualsNode(Token token);
        inline static const string type = "BoolNotEqualsNode";
        virtual string getType() override { return this->type; };
        virtual BoolNotEqualsNode * clone() override { return new BoolNotEqualsNode(*this); };
    private:
};

class BoolLessThanNode : public BoolExpressionNode {
    public:
        BoolLessThanNode(Token token);
        inline static const string type = "BoolLessThanNode";
        virtual string getType() override { return this->type; };
        virtual BoolLessThanNode * clone() override { return new BoolLessThanNode(*this); };
    private:
};

class BoolLessThanOrEqualNode : public BoolExpressionNode {
    public:
        BoolLessThanOrEqualNode(Token token);
        inline static const string type = "BoolLessThanOrEqualNode";
        virtual string getType() override { return this->type; };
        virtual BoolLessThanOrEqualNode * clone() override { return new BoolLessThanOrEqualNode(*this); };
    private:
};

class BoolGreaterThanNode : public BoolExpressionNode {
    public:
        BoolGreaterThanNode(Token token);
        inline static const string type = "BoolGreaterThanNode";
        virtual string getType() override { return this->type; };
        virtual BoolGreaterThanNode * clone() override { return new BoolGreaterThanNode(*this); };
    private:
};

class BoolGreaterThanOrEqualNode : public BoolExpressionNode {
    public:
        BoolGreaterThanOrEqualNode(Token token);
        inline static const string type = "BoolGreaterThanOrEqualNode";
        virtual string getType() override { return this->type; };
        virtual BoolGreaterThanOrEqualNode * clone() override { return new BoolGreaterThanOrEqualNode(*this); };
    private:
};

class BitShiftLeftNode : public BitExpressionNode {
    public:
        BitShiftLeftNode(Token token);
        inline static const string type = "BitShiftLeftNode";
        virtual string getType() override { return this->type; };
        virtual BitShiftLeftNode * clone() override { return new BitShiftLeftNode(*this); };
    private:
};

class BitShiftRightNode : public BitExpressionNode {
    public:
        BitShiftRightNode(Token token);
        inline static const string type = "BitShiftRightNode";
        virtual string getType() override { return this->type; };
        virtual BitShiftRightNode * clone() override { return new BitShiftRightNode(*this); };
    private:
};

class ModulusExpressionNode : public BinExpressionNode {
    public:
        ModulusExpressionNode(Token token);
        inline static const string type = "ModulusExpressionNode";
        virtual string getType() override { return this->type; };
        virtual ModulusExpressionNode * clone() override { return new ModulusExpressionNode(*this); };
    private:
};

class ExponentExpressionNode : public BinExpressionNode {
    public:
        ExponentExpressionNode(Token token);
        inline static const string type = "ExponentExpressionNode";
        virtual string getType() override { return this->type; };
        virtual ExponentExpressionNode * clone() override { return new ExponentExpressionNode(*this); };
    private:
};

class BitAndExpressionNode : public BitExpressionNode {
    public:
        BitAndExpressionNode(Token token);
        inline static const string type = "BitAndExpressionNode";
        virtual string getType() override { return this->type; };
        virtual BitAndExpressionNode * clone() override { return new BitAndExpressionNode(*this); };
    private:
};

class BitOrExpressionNode : public BitExpressionNode {
    public:
        BitOrExpressionNode(Token token);
        inline static const string type = "BitOrExpressionNode";
        virtual string getType() override { return this->type; };
        virtual BitOrExpressionNode * clone() override { return new BitOrExpressionNode(*this); };
    private:
};

class BitXorExpressionNode : public BitExpressionNode {
    public:
        BitXorExpressionNode(Token token);
        inline static const string type = "BitXorExpressionNode";
        virtual string getType() override { return this->type; };
        virtual BitXorExpressionNode * clone() override { return new BitXorExpressionNode(*this); };
    private:
};

class IfStatementNode : public AST {
    public:
        IfStatementNode(Token token);
        inline static const string type = "IfStatementNode";
        virtual string getType() override { return this->type; };
        virtual IfStatementNode * clone() override { return new IfStatementNode(*this); };
    private:
};

class IfConditionBlockNode : public AST {
    public:
        IfConditionBlockNode(Token token);
        inline static const string type = "IfConditionBlockNode";
        virtual string getType() override { return this->type; };
        virtual IfConditionBlockNode * clone() override { return new IfConditionBlockNode(*this); };
    private:
};

class ImportNode : public AST {
    public:
        ImportNode(Token token);
        inline static const string type = "ImportNode";
        virtual string getType() override { return this->type; };
        virtual ImportNode * clone() override { return new ImportNode(*this); };
    private:
};

class ImportedModuleNode : public AST {
    public:
        ImportedModuleNode(Token token);
        inline static const string type = "ImportedModuleNode";
        virtual string getType() override { return this->type; };
        virtual ImportedModuleNode * clone() override { return new ImportedModuleNode(*this); };
    private:
};


class FromImportNode : public AST {
    public:
        FromImportNode(Token token);
        inline static const string type = "FromImportNode";
        virtual string getType() override { return this->type; };
        virtual FromImportNode * clone() override { return new FromImportNode(*this); };
    private:
};

class TryStatementNode : public AST {
    public:
        TryStatementNode(Token token);
        inline static const string type = "TryStatementNode";
        virtual string getType() override { return this->type; };
        virtual TryStatementNode * clone() override { return new TryStatementNode(*this); };
    private:
};

class CatchStatementNode : public AST {
    public:
        CatchStatementNode(Token token);
        inline static const string type = "CatchStatementNode";
        virtual string getType() override { return this->type; };
        virtual CatchStatementNode * clone() override { return new CatchStatementNode(*this); };
    private:
};

class RaiseStatementNode : public AST {
    public:
        RaiseStatementNode(Token token);
        inline static const string type = "RaiseStatementNode";
        virtual string getType() override { return this->type; };
        virtual RaiseStatementNode * clone() override { return new RaiseStatementNode(*this); };
    private:
};

class AsNode : public AST {
    public:
        AsNode(Token token);
        inline static const string type = "AsNode";
        virtual string getType() override { return this->type; };
        virtual AsNode * clone() override { return new AsNode(*this); };
    private:
};

class ForStatementNode : public AST {
    public:
        ForStatementNode(Token token);
        inline static const string type = "ForStatementNode";
        virtual string getType() override { return this->type; };
        virtual ForStatementNode * clone() override { return new ForStatementNode(*this); };
    private:
};

class EachStatementNode : public AST {
    public:
        EachStatementNode(Token token);
        inline static const string type = "EachStatementNode";
        virtual string getType() override { return this->type; };
        virtual EachStatementNode * clone() override { return new EachStatementNode(*this); };
    private:
};

class WhileStatementNode : public AST {
    public:
        WhileStatementNode(Token token);
        inline static const string type = "WhileStatementNode";
        virtual string getType() override { return this->type; };
        virtual WhileStatementNode * clone() override { return new WhileStatementNode(*this); };
    private:
};

class SkipStatementNode : public AST {
    public:
        SkipStatementNode(Token token);
        inline static const string type = "SkipStatementNode";
        virtual string getType() override { return this->type; };
        virtual SkipStatementNode * clone() override { return new SkipStatementNode(*this); };
    private:
};

#endif  // BRIOLANG_SRC_TREE_H_