#ifndef BRIOLANG_SRC_LEXER_H_
#define BRIOLANG_SRC_LEXER_H_

#include <brio_lang/token.h>
#include <string>

using namespace std;

class Lexer {
    public:
        virtual Token nextToken();
        virtual void consume();
    private:
};

class BrioLexer : public Lexer {
    public:
        BrioLexer(string code);
        Token nextToken();
        void consume();
        void match(string target);
        char peek(int depth);
        void whitespace();
        bool isNumber();
        Token number();
        bool isLetter();
        int getCount(string s, char match);
        Token identifier();
        Token stringQuote(string start_char);
        void singleLineComment();
        void multiLineComment();
        bool isBoolean(string value);
        bool isNone(string value);
        bool isKeyword(string value);
        void reset();
    private:
        string code;
        int index;  // overall index
        string character;
        int lineno;
        int offset;  // line-specific index
};

#endif  // BRIOLANG_SRC_LEXER_H_