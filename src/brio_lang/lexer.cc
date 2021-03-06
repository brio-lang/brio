///
/// Brio Lang
/// lexer.cc
///

#include <brio_lang/lexer.h>
#include <brio_lang/token.h>
#include <brio_lang/exceptions.h>
#include <iostream>
#include <string>


const string C_EOF = "EOF";


BrioLexer::BrioLexer(string text){
    code = text;
    index = 0;
    character = text[index];
    lineno = 1;
    offset = 0;
};

Token Lexer::nextToken(){
    throw LexerError("lexer base class nextToken called");
}

void Lexer::consume(){
    throw LexerError("lexer base class consume called");
}

/**
 * Returns the next Token from the lexer.
 */
Token BrioLexer::nextToken(){
    while (character != C_EOF){
        if (character == " " || character == "\t" || character == "\n" || character == "\r"){
            this->whitespace();
        }
        else if (character == "," || character == ";" || character == "{" || character == "}" ||
                 character == "[" || character == "]" || character == "(" || character == ")" ||
                 character == ":"){
            string chr = character;
            this->consume();
            return Token(T_DELIMITER, chr, lineno, offset, index);
        }
        else if (character == "=" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "==", lineno, offset, index);
        }
        else if (character == "!" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "!=", lineno, offset, index);
        }
        else if (character == "+" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "+=", lineno, offset, index);
        }
        else if (character == "-" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "-=", lineno, offset, index);
        }
        else if (character == "*" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "*=", lineno, offset, index);
        }
        else if (character == "/" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "/=", lineno, offset, index);
        }
        else if (character == "|" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "|=", lineno, offset, index);
        }
        else if (character == "&" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "&=", lineno, offset, index);
        }
        else if (character == ">" && this->peek(1) == '>'){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, ">>", lineno, offset, index);
        }
        else if (character == "<" && this->peek(1) == '<'){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "<<", lineno, offset, index);
        }
        else if (character == ">" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, ">=", lineno, offset, index);
        }
        else if (character == "<" && this->peek(1) == '='){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "<=", lineno, offset, index);
        }
        else if (character == "*" && this->peek(1) == '*'){
            this->consume();
            this->consume();
            return Token(T_OPERATOR, "**", lineno, offset, index);
        }
        else if (character == "=" || character == "+" || character == "-" || character == "*" ||
                 character == "/" || character == ">" || character == "<" || character == "." ||
                 character == "%" || character == "&" || character == "|" || character == "^"){
            string chr = character;
            this->consume();
            return Token(T_OPERATOR, chr, lineno, offset, index);
        }
        else if (character == "\"" || character == "'"){
            return this->stringQuote(character);
        }
        else if (character == "#"){
            if (this->peek(1) == '#' && this->peek(2) == '#'){
                this->multiLineComment();
            }
            else{   
                this->singleLineComment();
            }
        }
        else if (character == "@"){
            this->consume();
            return Token(T_IDENTIFIER, "@", lineno, offset, index);
        }
        else if (this->isNumber()){
            return this->number();
        }
        else{
            if (this->isLetter() || character == "_"){
                return this->identifier();
            }
            // invalid token identified
            character = C_EOF;
        }
    };
    return Token(T_EOF, C_EOF, lineno, offset, index);
};

char BrioLexer::peek(int depth){
    if (index + depth > code.length()){
        return EOF;  // peeking beyond code length
    }
    return code[index + depth];
};

void BrioLexer::whitespace(){
    while (character == " " || character == "\t" || character == "\n" || character == "\r"){
        // track the source line number and offset
        if (character == "\n"){
            lineno += 1;
            offset = 0;
        }
        this->consume();
    }
};

void BrioLexer::consume(){
    index += 1;
    offset += 1;
    if (index >= code.length()){
        character = C_EOF;
    }else{
        character = code[index];
    }
};

void BrioLexer::reset(){
    this->index = 0;
    this->offset = 0;
    this->lineno = 1;
    this->character = this->code[0];
}

void BrioLexer::match(string target){
    if (character == target){
        this->consume();
    }
    throw LexerError("expected " + target + ", found " + character, lineno);
};

bool BrioLexer::isNumber(){
    if (isdigit(character[0]) > 0) return true;
    return false;
};

Token BrioLexer::number(){
    string number;

    while (this->isNumber() == true || character == "."){    
        number.append(character);
        this->consume();
    }

    if (this->getCount(number, '.') == 1){
        return Token(T_LITERAL_DECIMAL, number, lineno, offset, index);
    }
    else if (this->getCount(number, '.') > 1){
        throw LexerError("invalid decimal literal", lineno);
    }

    return Token(T_LITERAL_INT, number, lineno, offset, index);
};

int BrioLexer::getCount(string s, char match){
    int count = 0;

    for (int i = 0; i < s.size(); i++)
        if (s[i] == match) count++;

    return count;
};

bool BrioLexer::isLetter(){
    return isalpha(character[0]);
};

bool BrioLexer::isBoolean(string value){
    return (value == "true" || value == "false");
};

bool BrioLexer::isNone(string value){
    return value == "none";
};

Token BrioLexer::identifier(){
    string identifier;
    while (this->isLetter() || this->isNumber() || character == "_"){
        if (character == C_EOF) break;
        identifier.append(character);
        this->consume();
    }

    if (this->isBoolean(identifier)){
        return Token(T_LITERAL_BOOL, identifier, lineno, offset, index);
    }

    if (this->isNone(identifier)){
        return Token(T_LITERAL_NONE, identifier, lineno, offset, index);
    }

    if (this->isKeyword(identifier)){
        return Token(T_KEYWORD, identifier, lineno, offset, index);
    }

    return Token(T_IDENTIFIER, identifier, lineno, offset, index);
};

Token BrioLexer::stringQuote(string start_character){
    string quote;
    this->consume();
    while (character != start_character){
        if (character == C_EOF){
            throw LexerError("invalid string literal", lineno);
        }
        quote.append(character);
        this->consume();
    }
    this->consume();
    return Token(T_LITERAL_STRING, quote, lineno, offset, index);
};

void BrioLexer::singleLineComment(){
    this->consume();
    while (character != "\n" && character != C_EOF){
        this->consume();
    }
};

void BrioLexer::multiLineComment(){
    // consume the initial '###'
    this->consume(); this->consume(); this->consume();

    while (!(character == "#" && this->peek(1) == '#' && this->peek(2) == '#')){
        if (character == C_EOF){
            throw LexerError("invalid multi-line comment", lineno);
        }
        this->consume();
    }

    // consume the final '###'
    this->consume(); this->consume(); this->consume();
};

bool BrioLexer::isKeyword(string value){
    return Keywords::isKeyword(value);
};

