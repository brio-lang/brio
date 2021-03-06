#ifndef BRIOLANG_SRC_TOKEN_H_
#define BRIOLANG_SRC_TOKEN_H_

#include <string>

using namespace std;

const int T_EOF = 0;                    // 'None'
const int T_KEYWORD = 1;                // 'method'
const int T_IDENTIFIER = 2;             // 'foo'
const int T_OPERATOR = 3;               // '+'
const int T_DELIMITER = 4;              // '{'
const int T_LITERAL_INT = 5;            // 1
const int T_LITERAL_DECIMAL = 6;        // 3.14
const int T_LITERAL_STRING = 7;         // "hello"
const int T_LITERAL_BOOL = 8;           // "true" or "false"
const int T_LITERAL_NONE = 9;           // "none"
const int T_SINGLE_LINE_COMMENT = 10;   // '#'
const int T_MULTI_LINE_COMMENT = 11;    // '###'


class Keywords {
    public:
        inline static const string CLASS = "class";
        inline static const string LET = "let";
        inline static const string METHOD = "method";
        inline static const string NEW = "new";
        inline static const string RETURN = "return";
        inline static const string IF = "if";
        inline static const string ELSEIF = "elseif";
        inline static const string ELSE = "else";
        inline static const string WHILE = "while";
        inline static const string FOR = "for";
        inline static const string SKIP = "skip";
        inline static const string AND = "and";
        inline static const string OR = "or";
        inline static const string PUBLIC = "public";
        inline static const string PRIVATE = "private";
        inline static const string PROTECTED = "protected";
        inline static const string TRY = "try";
        inline static const string CATCH = "catch";
        inline static const string RAISE = "raise";
        inline static const string AS = "as";
        inline static const string IMPORT = "import";
        inline static const string FROM = "from";
        inline static const string EACH = "each";
        static bool isModifier(string value);
        static bool isKeyword(string value);
    private:
};

class Literals{
    public:
        inline static const string TRUE = "true";
        inline static const string FALSE = "false";
        inline static const string NONE = "none";
        inline static const string THIS = "@";
        inline static const string SUPER = "super";
    private:
};

class Token {
    public:
        Token();
        Token(int type, string text, int lineno, int offset, int index);
        string getName();
        int getType();
        string getText();
        void setText(string text);
        int getIndex();
        int getLineNo();
        int getOffset();
        int getBindingPower();
    private:
        int token_type;
        string token_text;
        int token_lineno;
        int token_offset;
        int token_index;
};

#endif  // BRIOLANG_SRC_TOKEN_H_