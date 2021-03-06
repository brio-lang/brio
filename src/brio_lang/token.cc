///
/// Brio Lang
/// token.cc
///

#include <brio_lang/token.h>
#include <string>


Token::Token(){
    token_type = T_EOF;
    token_type = -1;
    token_lineno = 0;
    token_offset = 0;
    token_index = 1;
};

Token::Token(int type, string text, int lineno, int offset, int index){
    token_type = type;
    token_text = text;
    token_lineno = lineno;
    token_offset = offset;
    token_index = index;
};

string Token::getText(){
    return token_text;
}

void Token::setText(string text){
    this->token_text = text;
}

int Token::getIndex(){
    return token_index;
}

/**
 * Determines the line number where the token was identified.
 */
int Token::getLineNo(){
    return token_lineno;
}

/**
 * Determines the offset index for the line where the token was found.
 */
int Token::getOffset(){
    return token_offset;
}

/**
 * Returns the integer token type.
 * @returns int token type
 */
int Token::getType(){
    return token_type;
}

/** Returns the binding power for operator precendence. **/
int Token::getBindingPower(){
    if (this->getType() == T_OPERATOR){
        if (this->getText() == "=") return 10;
        if (this->getText() == "+=") return 10;
        if (this->getText() == "-=") return 10;
        if (this->getText() == "*=") return 10;
        if (this->getText() == "/=") return 10;
        if (this->getText() == "|=") return 10;
        if (this->getText() == "&=") return 10;
        if (this->getText() == "or") return 24;
        if (this->getText() == "and") return 25;
        if (this->getText() == "<") return 30;
        if (this->getText() == ">") return 30;
        if (this->getText() == "<=") return 30;
        if (this->getText() == ">=") return 30;
        if (this->getText() == "==") return 30;
        if (this->getText() == "!=") return 30;
        if (this->getText() == "|") return 33;
        if (this->getText() == "^") return 34;
        if (this->getText() == "&") return 35;
        if (this->getText() == "<<") return 40;
        if (this->getText() == ">>") return 40;
        if (this->getText() == "+") return 50;
        if (this->getText() == "-") return 50;
        if (this->getText() == "%") return 60;
        if (this->getText() == "*") return 60;
        if (this->getText() == "/") return 60;
        if (this->getText() == "**") return 70;
        if (this->getText() == ".") return 80;
    }
    return 100;  // the highest bp
}

/**
 * Returns the string name for the token based on the type.
 * @returns string token name e.g. "IDENTIFIER"
 */
string Token::getName(){
    switch (token_type){
        case T_EOF:
            return "EOF";
        case T_KEYWORD:
            return "KEYWORD";
        case T_IDENTIFIER:
            return "IDENTIFIER";
        case T_OPERATOR:
            return "OPERATOR";
        case T_DELIMITER:
            return "DELIMITER";
        case T_LITERAL_INT:
            return "LITERAL_INT";
        case T_LITERAL_DECIMAL:
            return "LITERAL_DECIMAL";
        case T_LITERAL_STRING:
            return "LITERAL_STRING";
        case T_LITERAL_BOOL:
            return "LITERAL_BOOL";
        case T_LITERAL_NONE:
            return "LITERAL_NONE";
        case T_SINGLE_LINE_COMMENT:
            return "SINGLE LINE COMMENT";
        case T_MULTI_LINE_COMMENT:
            return "MULTI_LINE_COMMENT";
    }
    return "INVALID";
};

/**
 * Returns true or false to indicate if value is modifier.
 */
bool Keywords::isModifier(string value){
    return (
        value == Keywords::PRIVATE ||
        value == Keywords::PUBLIC ||
        value == Keywords::PROTECTED
    );
};

/**
 * Returns true or false to indicate if value is keyword.
 */
bool Keywords::isKeyword(string value){
    return (
        value == Keywords::CLASS || 
        value == Keywords::LET || 
        value == Keywords::METHOD || 
        value == Keywords::NEW ||
        value == Keywords::RETURN || 
        value == Keywords::IF || 
        value == Keywords::ELSEIF ||
        value == Keywords::ELSE || 
        value == Keywords::SKIP || 
        value == Keywords::AND || 
        value == Keywords::OR ||
        value == Keywords::PROTECTED || 
        value == Keywords::PUBLIC || 
        value == Keywords::PRIVATE ||
        value == Keywords::TRY || 
        value == Keywords::CATCH || 
        value == Keywords::RAISE ||  
        value == Keywords::AS ||
        value == Keywords::WHILE ||
        value == Keywords::FOR ||
        value == Keywords::IMPORT ||
        value == Keywords::FROM ||
        value == Keywords::EACH
    );
}