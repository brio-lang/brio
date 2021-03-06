#include <gtest/gtest.h>

#include <brio_lang/interpreter.h>
#include <brio_lang/parser.h>
#include <brio_lang/lexer.h>
#include <brio_lang/visitor.h>
#include <brio_lang/def.h>
#include <brio_lang/ref.h>

Object * exec(string code){
    BrioLexer lexer(code);
    BrioParser parser(&lexer);
    ProgramNode tree = parser.program();

    // enforce operator precedence
    OperatorPrecedenceVisitor opVisitor;
    opVisitor.visit(&tree);

    SymbolTable symbol_table;
    Def def(&symbol_table);
    def.downUp(&tree);

    Ref ref(&symbol_table);
    ref.downUp(&tree);

    // execute the tree-based interpreter
    Interpreter interpreter(&tree, &symbol_table);
    return interpreter.exec(tree.children[0]);
};

TEST(InterpreterTest, IntegerAddition){
    Object * result = exec("1 + 1");
    ASSERT_EQ(result->toString(), "2");
};

TEST(InterpreterTest, IntegerSubtraction){
    Object * result = exec("1 - 1");
    ASSERT_EQ(result->toString(), "0");
};

TEST(InterpreterTest, IntegerMultiplication){
    Object * result = exec("2 * 2");
    ASSERT_EQ(result->toString(), "4");
};

TEST(InterpreterTest, IntegerDivision){
    Object * result = exec("2 / 2");
    ASSERT_EQ(result->toString(), "1");
};

TEST(InterpreterTest, IntegerExponent){
    Object * result = exec("2 ** 2");
    ASSERT_EQ(result->toString(), "4");
};

TEST(InterpreterTest, IntegerModulus){
    Object * result = exec("2 % 4");
    ASSERT_EQ(result->toString(), "2");
};

TEST(InterpreterTest, IntegerNegativeExponent){
    Object * result = exec("2 ** -2");
    ASSERT_EQ(result->toString(), "0.25");
};

// TEST(InterpreterTest, OpPrecedence){
//     Object * result = exec("3 * 3 - -2 * (3 + -1 * (10)) - 5 + 2 ** -2");
//     ASSERT_EQ(result->toString(), "-9.75");
// };