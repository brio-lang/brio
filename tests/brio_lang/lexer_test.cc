#include <gtest/gtest.h>

#include <brio_lang/lexer.h>
#include <brio_lang/token.h>

#include <string>


TEST(LexerTest, TokenKeyword){
    BrioLexer lexer("let");
    ASSERT_EQ(lexer.nextToken().getType(), T_KEYWORD);
    ASSERT_EQ(lexer.nextToken().getType(), T_EOF);
};

TEST(LexerTest, TokenIdentifier){
    BrioLexer lexer("foo");
    ASSERT_EQ(lexer.nextToken().getType(), T_IDENTIFIER);
};

TEST(LexerTest, TokenOperator){
    BrioLexer lexer("=");
    ASSERT_EQ(lexer.nextToken().getType(), T_OPERATOR);
};

TEST(LexerTest, TokenDelimiter){
    BrioLexer lexer(";");
    ASSERT_EQ(lexer.nextToken().getType(), T_DELIMITER);
};

TEST(LexerTest, TokenLiteralInt){
    BrioLexer lexer("1");
    ASSERT_EQ(lexer.nextToken().getType(), T_LITERAL_INT);
};

TEST(LexerTest, TokenLiteralString){
    BrioLexer lexer("'hello'");
    ASSERT_EQ(lexer.nextToken().getType(), T_LITERAL_STRING);
};

TEST(LexerTest, TokenLiteralBool){
    BrioLexer lexer("false");
    ASSERT_EQ(lexer.nextToken().getType(), T_LITERAL_BOOL);
};

TEST(LexerTest, HelloWorld){
    string code = "print('hello world');";
    BrioLexer lexer(code);
    ASSERT_EQ(lexer.nextToken().getType(), T_IDENTIFIER);
    ASSERT_EQ(lexer.nextToken().getType(), T_DELIMITER);
    ASSERT_EQ(lexer.nextToken().getType(), T_LITERAL_STRING);
    ASSERT_EQ(lexer.nextToken().getType(), T_DELIMITER);
    ASSERT_EQ(lexer.nextToken().getType(), T_DELIMITER);
    ASSERT_EQ(lexer.nextToken().getType(), T_EOF);
};

TEST(LexerTest, VarAssignment){
    string code = "let x = 1 + 1;";
    BrioLexer lexer(code);
    ASSERT_EQ(lexer.nextToken().getType(), T_KEYWORD);
    ASSERT_EQ(lexer.nextToken().getType(), T_IDENTIFIER);
    ASSERT_EQ(lexer.nextToken().getType(), T_OPERATOR);
    ASSERT_EQ(lexer.nextToken().getType(), T_LITERAL_INT);
    ASSERT_EQ(lexer.nextToken().getType(), T_OPERATOR);
    ASSERT_EQ(lexer.nextToken().getType(), T_LITERAL_INT);
    ASSERT_EQ(lexer.nextToken().getType(), T_DELIMITER);
    ASSERT_EQ(lexer.nextToken().getType(), T_EOF);
};