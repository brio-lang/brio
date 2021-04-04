#ifndef BRIOLANG_SRC_PARSER_H_
#define BRIOLANG_SRC_PARSER_H_

#include <brio_lang/lexer.h>
#include <brio_lang/tree.h>
#include <brio_lang/token.h>
#include <vector>


class Parser {
    public:
        Parser(Lexer * lex);
        void consume();
        int la(int i);
        Token lt(int i);
        Token next(string text);
        void match(int token_type, string value);
        void match(int token_type);
        void sync(int i);
        void fill(int i);
        int mark();
        void release();
        void seek(int index);
        bool isSpeculating();
        int pIndex();
        void memoize(map<int,int> memoization, int startIndex, bool failed);
        bool alreadyParsedRule(map<int,int> memoization);
        virtual inline void clearMemo(){};
    private:
        Lexer * lexer;
        std::vector<Token> lookahead;
        std::vector<int> markers;
        int p;
};

class BrioParser : public Parser {
    public:
        BrioParser(BrioLexer * lex);
        void addNodeAndDescend(AST * node);
        void addChild(AST * node);
        AST * popChild();
        void restoreNode();
        void clearMemo();
        ProgramNode program();
        void import();
        void importFrom();
        void methodDeclaration();
        bool speculateMethodDeclaration();
        void methodCall();
        bool speculateMethodCall();
        void classDeclaration();
        bool speculateClassDeclaration();
        void variableDeclaration();
        void variableAssignment();
        bool speculateVariableAssignment();
        void classInstantiation();
        void ifStatement();
        void whileStatement();
        void forStatement();
        void eachStatement();
        void skipStatement();
        void tryStatement();
        void catchStatement();
        void raiseStatement();
        void as();
        void identifier();
        void memberAccess();
        bool speculateMemberAccess();
        void literalInt();
        void literalIntNegative();
        void literalDecimal();
        void literalDecimalNegative();
        void literalString();
        void literalBoolean();
        void literalNone();
        void optEndStatement();
        void index();
        ModifierNode * optModifiers();
        void optSuperClass();
        void paramsList();
        ParenthesesNode * parentheses();
        void lessThan();
        void lessThan(ParenthesesNode * node);
        bool speculateLessThan();
        void lessThanOrEqual();
        void lessThanOrEqual(ParenthesesNode * node);
        bool speculateLessThanOrEqual();
        void greaterThan();
        void greaterThan(ParenthesesNode * node);
        bool speculateGreaterThan();
        void greaterThanOrEqual();
        void greaterThanOrEqual(ParenthesesNode * node);
        bool speculateGreaterThanOrEqual();
        void equalTo();
        void equalTo(ParenthesesNode * node);
        bool speculateEqualTo();
        void notEqualTo();
        void notEqualTo(ParenthesesNode * node);
        bool speculateNotEqualTo();
        void block();
        void shiftLeft();
        void shiftLeft(ParenthesesNode * node);
        bool speculateShiftLeft();
        void shiftRight();
        void shiftRight(ParenthesesNode * node);
        bool speculateShiftRight();
        void boolAnd();
        void boolAnd(ParenthesesNode * node);
        bool speculateBoolAnd();
        void boolOr();
        void boolOr(ParenthesesNode * node);
        bool speculateBoolOr();
        void bitXor();
        void bitXor(ParenthesesNode * node);
        bool speculateBitXor();
        void bitOr();
        void bitOr(ParenthesesNode * node);
        bool speculateBitOr();
        void bitOrAssign();
        bool speculateBitOrAssign();
        void bitAnd();
        void bitAnd(ParenthesesNode * node);
        bool speculateBitAnd();
        void bitAndAssign();
        bool speculateBitAndAssign();
        void binaryOperation();
        void binaryOperation(ParenthesesNode * node);
        void exponent();
        void exponent(ParenthesesNode * node);
        bool speculateExponent();
        void addition();
        void addition(ParenthesesNode * node);
        bool speculateAddition();
        void additionAssign();
        bool speculateAdditionAssign();
        void subtraction();
        void subtraction(ParenthesesNode * node);
        bool speculateSubtraction();
        void subtractionAssign();
        bool speculateSubtractionAssign();
        void multiplication();
        void multiplication(ParenthesesNode * node);
        bool speculateMultiplication();
        void multiplicationAssign();
        bool speculateMultiplicationAssign();
        void division();
        void division(ParenthesesNode * node);
        bool speculateDivision();
        void divisionAssign();
        bool speculateDivisionAssign();
        void modulus();
        void modulus(ParenthesesNode * node);
        bool speculateModulus();
        void test();
        void term();
        void returnValue();
        void array();
        void dictionary();
        void expression();
        void statement();
    private:
        AST * rootNode;
        AST * currentNode;
        std::vector<AST*> saveNodes;

        // data structures for memoization
        map<int,int> methodCallMemo;
        map<int,int> memberAccessMemo;
        map<int,int> methodDeclarationMemo;
        map<int,int> classDeclarationMemo;
        map<int,int> varAssignmentMemo;
        map<int,int> lessThanMemo;
        map<int,int> lessThanOrEqualMemo;
        map<int,int> greaterThanMemo;
        map<int,int> greaterThanOrEqualMemo;
        map<int,int> equalToMemo;
        map<int,int> notEqualToMemo;
        map<int,int> shiftLeftMemo;
        map<int,int> shiftRightMemo;
        map<int,int> boolAndMemo;
        map<int,int> boolOrMemo;
        map<int,int> boolXorMemo;
        map<int,int> bitOrMemo;
        map<int,int> bitXorMemo;
        map<int,int> bitOrAssignMemo;
        map<int,int> bitAndMemo;
        map<int,int> bitAndAssignMemo;
        map<int,int> exponentMemo;
        map<int,int> additionMemo;
        map<int,int> additionAssignMemo;
        map<int,int> subtractionMemo;
        map<int,int> subtractionAssignMemo;
        map<int,int> multiplicationMemo;
        map<int,int> multiplicationAssignMemo;
        map<int,int> divisionMemo;
        map<int,int> divisionAssignMemo;
        map<int,int> modulusMemo;
};

#endif  // BRIOLANG_SRC_PARSER_H_