///
/// Brio Lang
/// parser.cc
///
/// Backtracking recursive descent parser with memoization.
/// Dynamically sized lookahead buffer.
///

#include <brio_lang/parser.h>
#include <brio_lang/lexer.h>
#include <brio_lang/tree.h>
#include <brio_lang/token.h>
#include <brio_lang/exceptions.h>

#include <iostream>


const int FAILED = -1;  // parsing failed last attempt

Parser::Parser(Lexer * lex){
    lexer = lex;
    p = 0;
    sync(1);  // prime buffer with at least one token
};

void Parser::consume(){
    p++;
    if (p == lookahead.size() && !isSpeculating()){
        p = 0;
        lookahead.clear();
        clearMemo();
    }
    sync(1);  // get another to replace consumed token
};

/**
 * Returns lookahead token types for i symbol
 */
int Parser::la(int i){
    return this->lt(i).getType();
};

/**
 * Returns actual lookahead token at a particular depth
 */
Token Parser::lt(int i){
    sync(i);
    return lookahead[p + i - 1];
};

/** 
 * Returns the next lookahead token that matches input string.
 */
Token Parser::next(string text){
    for (int i=1; i<=10; i++){
        if (this->lt(i).getText() == text){
            return this->lt(i);
        }
    }
    throw RecognitionException("token '" + text + "' not found in lookahead");
};

/**
 * Returns the current input position. 
 */
int Parser::pIndex(){
    return p;
}

void Parser::memoize(map<int,int> memoization, int startIndex, bool failed){
    int stopIndex = failed ? FAILED : this->pIndex();
    memoization[startIndex] = stopIndex;
};

bool Parser::alreadyParsedRule(map<int,int> memoization){
    if (!memoization.count(this->pIndex())) return false;
    int memo = memoization[this->pIndex()];
    if (memo == FAILED){
        throw PreviousParseFailedException();
    }
    seek(memo);
    return true;
}

void Parser::match(int token_type, string value){
    if (this->la(1) == token_type && this->lt(1).getText() == value){
        this->consume();
    }else{
        int lineno = this->lt(1).getLineNo();
        throw MismatchedTokenException("expecting '" + value + "' at offset " + to_string(this->lt(1).getOffset()), lineno);
    }
};

void Parser::match(int token_type){
    if (this->la(1) == token_type){
        this->consume();
    }else{
        int lineno = this->lt(1).getLineNo();
        throw MismatchedTokenException("expecting type " + this->lt(1).getName() + " at offset " + to_string(this->lt(1).getOffset()), lineno);
    }
};

void Parser::sync(int i){
    if ( p+i-1 > (int(lookahead.size()) - 1) ){
        int n = (p+i-1) - (int(lookahead.size())-1);
        fill(n);
    }
};

void Parser::fill(int n){
    for (int i=1; i <= n; i++){
        lookahead.push_back(lexer->nextToken());
    }
};

int Parser::mark(){
    markers.push_back(p);
    return p;
};

void Parser::release(){
    int marker = markers[int(markers.size()) - 1]; 
    markers.pop_back();
    seek(marker);
};

void Parser::seek(int index){
    p = index;
};

bool Parser::isSpeculating(){
    return int(markers.size()) > 0;
};

BrioParser::BrioParser(BrioLexer * lex) : 
    Parser(lex) {};

void BrioParser::addNodeAndDescend(AST * node){
    if (isSpeculating()) return;
    this->addChild(node);
    saveNodes.push_back(currentNode);
    currentNode = node;
};

void BrioParser::addChild(AST * node){
    if (isSpeculating()) return;
    this->currentNode->addChild(node);
}

void BrioParser::clearMemo(){
    methodCallMemo.clear();
    memberAccessMemo.clear();
    methodDeclarationMemo.clear();
    classDeclarationMemo.clear();
    varAssignmentMemo.clear();
    lessThanMemo.clear();
    lessThanOrEqualMemo.clear();
    greaterThanMemo.clear();
    greaterThanOrEqualMemo.clear();
    equalToMemo.clear();
    notEqualToMemo.clear();
    shiftLeftMemo.clear();
    shiftRightMemo.clear();
    boolAndMemo.clear();
    boolOrMemo.clear();
    boolXorMemo.clear();
    bitOrMemo.clear();
    bitXorMemo.clear();
    bitOrAssignMemo.clear();
    bitAndMemo.clear();
    bitAndAssignMemo.clear();
    exponentMemo.clear();
    additionMemo.clear();
    additionAssignMemo.clear();
    subtractionMemo.clear();
    subtractionAssignMemo.clear();
    multiplicationMemo.clear();
    multiplicationAssignMemo.clear();
    divisionMemo.clear();
    divisionAssignMemo.clear();
    modulusMemo.clear();
}

AST * BrioParser::popChild(){
    if (isSpeculating()) return nullptr;
    return this->currentNode->popChild();
}

void BrioParser::restoreNode(){
    if (isSpeculating()) return;
    AST * node = saveNodes[int(saveNodes.size()) - 1];
    saveNodes.pop_back();
    currentNode = node;
};

ProgramNode BrioParser::program(){
    // create root program node
    ProgramNode pn;
    rootNode = &pn;
    currentNode = rootNode;

    // match statement rules
    while (this->la(1) != T_EOF){
        this->statement();
    }

    // return root of AST
    return pn;
};

void BrioParser::import(){
    ImportNode * node = new ImportNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match import statement rules
    this->match(T_KEYWORD, Keywords::IMPORT);
    this->term();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::importFrom(){
    FromImportNode * node = new FromImportNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match from import statement rules
    this->match(T_KEYWORD, Keywords::FROM);
    this->identifier();
    this->match(T_KEYWORD, Keywords::IMPORT);
    this->identifier();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

ModifierNode * BrioParser::optModifiers(){
    ModifierNode * modifiers = new ModifierNode(this->lt(1));;

    while (Keywords::isModifier(this->lt(1).getText())){
        LiteralStringNode * mod = new LiteralStringNode(this->lt(1));
        modifiers->addChild(mod);
        this->match(T_KEYWORD);
    }

    return modifiers;
};

void BrioParser::optEndStatement(){
    if (this->lt(1).getText() == ";"){
        this->match(T_DELIMITER, ";");
    }
};

bool BrioParser::speculateMethodDeclaration(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(methodDeclarationMemo)) return success;

    mark();
    try{
        this->methodDeclaration();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(methodDeclarationMemo, startIndex, !success);
    return success;
}

void BrioParser::methodDeclaration(){
    MethodDeclarationNode* node = new MethodDeclarationNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match method declaration rules
    ModifierNode* modifiers = this->optModifiers();
    this->match(T_KEYWORD, Keywords::METHOD);
    this->identifier();
    this->addChild(modifiers);
    this->paramsList();
    this->block();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateMethodCall(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(methodCallMemo)) return success;

    mark();
    try{
        this->methodCall();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(methodCallMemo, startIndex, !success);
    return success;
}

void BrioParser::methodCall(){
    MethodCallNode* node = new MethodCallNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match method call rules
    if (this->speculateMemberAccess()){
        this->memberAccess();
    } else {
        this->identifier();
    }
    this->paramsList();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::optSuperClass(){
    SuperClassNode* node = new SuperClassNode(this->lt(1)); 
    this->addNodeAndDescend(node);

    // match super class rules
    if (this->la(1) == T_DELIMITER && this->lt(1).getText() == ":"){
        this->match(T_DELIMITER, ":");
        this->identifier();

        // look for additional super classes
        while (this->lt(1).getText() == ","){
            this->match(T_DELIMITER, ",");
            this->identifier();
        }
    }

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateClassDeclaration(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(classDeclarationMemo)) return success;

    mark();
    try{
        this->classDeclaration();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(classDeclarationMemo, startIndex, !success);
    return success;
}

void BrioParser::classDeclaration(){
    ClassDeclarationNode* node = new ClassDeclarationNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match class declaration rules
    ModifierNode* modifiers = this->optModifiers();
    this->match(T_KEYWORD, Keywords::CLASS);
    this->identifier();
    this->addChild(modifiers);
    this->optSuperClass();
    this->block();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::variableDeclaration(){
    VarDeclarationNode* node = new VarDeclarationNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match variable assignment rules
    this->match(T_KEYWORD, Keywords::LET);
    this->identifier();

    if (this->lt(1).getType() == T_OPERATOR){
        // standard let x = 1 statement
        this->match(T_OPERATOR, "=");

        if (this->lt(1).getText() == Keywords::NEW){
            this->classInstantiation();
        }
        else {
            this->expression();
        }

        this->optEndStatement();
    }

    // restore node position
    this->restoreNode();

};

bool BrioParser::speculateVariableAssignment(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(varAssignmentMemo)) return success;

    mark();
    try{
        this->variableAssignment();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(varAssignmentMemo, startIndex, !success);
    return success;
}

void BrioParser::variableAssignment(){
    VarAssignmentNode* node = new VarAssignmentNode(this->lt(2));
    this->addNodeAndDescend(node);

    // match variable assignment rules
    this->term();
    this->match(T_OPERATOR, "=");
    this->expression();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

ParenthesesNode* BrioParser::parentheses(){
    ParenthesesNode* node = new ParenthesesNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match parentheses rules
    this->match(T_DELIMITER, "(");
    while (this->lt(1).getText() != ")"){
        this->test();
    }
    this->match(T_DELIMITER, ")");

    // restore node position
    this->restoreNode();
    return dynamic_cast<ParenthesesNode*>(this->popChild());
};

void BrioParser::paramsList(){
    ParamsListNode* node = new ParamsListNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match params list rules
    this->match(T_DELIMITER, "(");
    while (this->lt(1).getText() != ")"){
        this->expression();

        if (this->lt(1).getText() == ","){
            this->match(T_DELIMITER, ",");
        }
    }
    this->match(T_DELIMITER, ")");

    // restore node position
    this->restoreNode();
};

void BrioParser::block(){
    BlockNode* node = new BlockNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match block rules
    this->match(T_DELIMITER, "{");
    while (this->lt(1).getText() != "}"){
        this->statement();
    }
    this->match(T_DELIMITER, "}");

    // restore node position
    this->restoreNode();
};

void BrioParser::returnValue(){
    ReturnNode* node = new ReturnNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match return rules
    this->match(T_KEYWORD, Keywords::RETURN);

    if (this->la(1) != T_DELIMITER){
        this->expression();
    }

    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::dictionary(){
    DictionaryNode* node = new DictionaryNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match dictionary rules
    this->match(T_DELIMITER, "{");
    while (this->lt(1).getText() != "}"){
        // create key value pair node
        DictionaryKeyValueNode* kv = new DictionaryKeyValueNode(this->lt(1));
        this->addNodeAndDescend(kv);

        this->expression();
        this->match(T_DELIMITER, ":");
        this->expression();

        if (this->lt(1).getText() == ","){
            this->match(T_DELIMITER, ",");
        }

        this->restoreNode();
    }
    this->match(T_DELIMITER, "}");

    // restore node position
    this->restoreNode();
};

void BrioParser::array(){
    ArrayNode* node = new ArrayNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match array list rules
    this->match(T_DELIMITER, "[");
    while (this->lt(1).getText() != "]"){
        this->expression();

        if (this->lt(1).getText() == ","){
            this->match(T_DELIMITER, ",");
        }
    }
    this->match(T_DELIMITER, "]");

    // restore node position
    this->restoreNode();
};

void BrioParser::classInstantiation(){
    ClassInstantiationNode* class_instantiation = new ClassInstantiationNode(this->lt(1));
    this->addNodeAndDescend(class_instantiation);

    // match class instantiation rules
    this->match(T_KEYWORD, Keywords::NEW);
    if (this->speculateMemberAccess()){
        this->memberAccess();
    }else{
        this->identifier();
    }
    this->paramsList();

    // restore node position
    this->restoreNode();
};

void BrioParser::literalInt(){
    LiteralIntNode* value = new LiteralIntNode(this->lt(1));
    this->addChild(value);
    this->match(T_LITERAL_INT);
};

void BrioParser::literalIntNegative(){
    // add sign to integer token
    Token signedInt = this->lt(2);
    signedInt.setText("-" + signedInt.getText());

    LiteralIntNode* value = new LiteralIntNode(signedInt);
    this->addChild(value);
    this->match(T_OPERATOR, "-");
    this->match(T_LITERAL_INT);
};

void BrioParser::literalDecimal(){
    LiteralDecimalNode* value = new LiteralDecimalNode(this->lt(1));
    this->addChild(value);
    this->match(T_LITERAL_DECIMAL);
};

void BrioParser::literalDecimalNegative(){
    // add sign to decimal token
    Token signedDecimal = this->lt(2);
    signedDecimal.setText("-" + signedDecimal.getText());

    LiteralDecimalNode* value = new LiteralDecimalNode(signedDecimal);
    this->addChild(value);
    this->match(T_OPERATOR, "-");
    this->match(T_LITERAL_DECIMAL);
};

void BrioParser::literalString(){
    LiteralStringNode* value = new LiteralStringNode(this->lt(1));
    this->addChild(value);
    this->match(T_LITERAL_STRING);
};

void BrioParser::literalBoolean(){
    LiteralBooleanNode* value = new LiteralBooleanNode(this->lt(1));
    this->addChild(value);
    this->match(T_LITERAL_BOOL);
};

void BrioParser::literalNone(){
    LiteralNoneNode* value = new LiteralNoneNode(this->lt(1));
    this->addChild(value);
    this->match(T_LITERAL_NONE);
};

void BrioParser::identifier(){
    IdentifierNode* id = new IdentifierNode(this->lt(1));
    this->addChild(id);
    this->match(T_IDENTIFIER);
};

void BrioParser::expression(){
    if (this->la(1) == T_IDENTIFIER && this->lt(2).getText() == Keywords::AS){
        this->as();
    }
    else if (this->lt(1).getText() == Keywords::NEW && this->la(2) == T_IDENTIFIER){
        this->classInstantiation();
    }
    else if (this->speculateVariableAssignment()){
        this->variableAssignment();
    }
    else if (this->speculateAdditionAssign()){
        this->additionAssign();
    }
    else if (this->speculateSubtractionAssign()){
        this->subtractionAssign();
    }
    else if (this->speculateMultiplicationAssign()){
        this->multiplicationAssign();
    }
    else if (this->speculateDivisionAssign()){
        this->divisionAssign();
    }
    else if (this->speculateBitOrAssign()){
        this->bitOrAssign();
    }
    else if (this->speculateBitAndAssign()){
        this->bitAndAssign();
    }
    else{
        this->test();
    }
};

void BrioParser::index(){
    IndexNode* node = new IndexNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match index rules
    this->match(T_DELIMITER, "[");
    this->expression();
    this->match(T_DELIMITER, "]");

    // restore node position
    this->restoreNode();
}

bool BrioParser::speculateMemberAccess(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(memberAccessMemo)) return success;

    mark();
    try{
        this->memberAccess();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(memberAccessMemo, startIndex, !success);
    return success;
};

void BrioParser::memberAccess(){
    // EX:  @board[0][1]  z["key"].foo  x[0]  y["foo"]  @board.foo.bar
    // EX:  @board[row][column]
    MemberAccessNode* node = new MemberAccessNode(this->lt(1));
    this->addNodeAndDescend(node);

    // support for @ keyword
    if (this->lt(1).getText() == Literals::THIS){
        this->identifier();  // @
        this->identifier();
    }
    else{
        this->identifier();
        if (this->lt(1).getText() != "." && this->lt(1).getText() != "["){
            throw RecognitionException("Invalid memberAccess");
        }
    }

    // parse subsequent parts
    bool nextItem = this->lt(1).getText() == "." || this->lt(1).getText() == "[";
    while(nextItem){
        if (this->lt(1).getText() == "."){
            this->match(T_OPERATOR, ".");
            this->identifier();
        }
        else if (this->lt(1).getText() == "["){
            this->index();
        }
        nextItem = this->lt(1).getText() == "." || this->lt(1).getText() == "[";
    }

    // restore node position
    this->restoreNode();
};

void BrioParser::tryStatement(){
    TryStatementNode* node = new TryStatementNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match try statement rules
    this->match(T_KEYWORD, Keywords::TRY);
    this->block();
    
    // confirm at least one catch follows the try block
    if (this->lt(1).getText() != Keywords::CATCH){
        throw SyntaxError("try statement must include a catch");
    }

    // parse subsequent catch blocks
    while (this->lt(1).getText() == Keywords::CATCH){
        this->catchStatement();
    }
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::catchStatement(){
    CatchStatementNode* node = new CatchStatementNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match catch statement rules
    this->match(T_KEYWORD, Keywords::CATCH);
    if (this->lt(1).getText() == "("){
        this->paramsList();
    }
    this->block();

    // restore node position
    this->restoreNode();
};

void BrioParser::raiseStatement(){
    RaiseStatementNode* node = new RaiseStatementNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match raise statement rules
    this->match(T_KEYWORD, Keywords::RAISE);
    this->identifier();
    if (this->lt(1).getText() == "("){
        this->paramsList();
    }
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::as(){
    AsNode* node = new AsNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match as node statement rules
    this->identifier();
    this->match(T_KEYWORD, Keywords::AS);
    this->identifier();

    // restore node position
    this->restoreNode();
};

void BrioParser::ifStatement(){
    IfStatementNode* node = new IfStatementNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match if statement rules
    IfConditionBlockNode* ifCondBlock = new IfConditionBlockNode(this->lt(1));
    this->addNodeAndDescend(ifCondBlock);

    this->match(T_KEYWORD, Keywords::IF);
    this->match(T_DELIMITER, "(");
    this->test();
    this->match(T_DELIMITER, ")");
    this->block();

    this->restoreNode();

    // loop for else if statements
    while (this->lt(1).getText() == Keywords::ELSEIF){
        IfConditionBlockNode* elseifCondBlock = new IfConditionBlockNode(this->lt(1));
        this->addNodeAndDescend(elseifCondBlock);
        
        this->match(T_KEYWORD, Keywords::ELSEIF);
        this->match(T_DELIMITER, "(");
        this->test();
        this->match(T_DELIMITER, ")");
        this->block();

        this->restoreNode();
    }

    // check for closing else statement
    if (this->lt(1).getText() == Keywords::ELSE){
        IfConditionBlockNode* elseCondBlock = new IfConditionBlockNode(this->lt(1));
        this->addNodeAndDescend(elseCondBlock);

        this->match(T_KEYWORD, Keywords::ELSE);
        this->block();

        this->restoreNode();
    }

    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::whileStatement(){
    WhileStatementNode* node = new WhileStatementNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match if statement rules
    this->match(T_KEYWORD, Keywords::WHILE);
    this->match(T_DELIMITER, "(");
    this->test();
    this->match(T_DELIMITER, ")");
    this->block();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::skipStatement(){
    SkipStatementNode* node = new SkipStatementNode(this->lt(1));
    this->addChild(node);

    // match skip statement rules
    this->match(T_KEYWORD, Keywords::SKIP);
    this->optEndStatement();
};

void BrioParser::eachStatement(){
    EachStatementNode* node = new EachStatementNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match each statement rules
    this->match(T_KEYWORD, Keywords::EACH);
    this->match(T_DELIMITER, "(");
    this->variableDeclaration();
    this->match(T_DELIMITER, ":");
    this->expression();
    this->match(T_DELIMITER, ")");
    this->block();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::forStatement(){
    ForStatementNode* node = new ForStatementNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match if statement rules
    this->match(T_KEYWORD, Keywords::FOR);
    this->match(T_DELIMITER, "(");
    this->variableDeclaration();
    this->test();
    this->match(T_DELIMITER, ";");
    this->statement();
    this->match(T_DELIMITER, ")");
    this->block();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateBoolAnd(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(boolAndMemo)) return success;

    mark();
    try{
        this->boolAnd();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(boolAndMemo, startIndex, !success);
    return success;
}

void BrioParser::boolAnd(){
    BoolAndNode* node = new BoolAndNode(this->next(Keywords::AND));
    this->addNodeAndDescend(node);

    // match boolean and statement rules
    this->term();
    this->match(T_KEYWORD, Keywords::AND);
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::boolAnd(ParenthesesNode * lhs){
    BoolAndNode* node = new BoolAndNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match boolean and statement rules
    this->addChild(lhs);
    this->match(T_OPERATOR, Keywords::AND);
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateBoolOr(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(boolOrMemo)) return success;

    mark();
    try{
        this->boolOr();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(boolOrMemo, startIndex, !success);
    return success;
}

void BrioParser::boolOr(){
    BoolOrNode* node = new BoolOrNode(this->next(Keywords::OR));
    this->addNodeAndDescend(node);

    // match boolean or statement rules
    this->term();
    this->match(T_KEYWORD, Keywords::OR);
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::boolOr(ParenthesesNode * lhs){
    BoolOrNode* node = new BoolOrNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match boolean or statement rules
    this->addChild(lhs);
    this->match(T_OPERATOR, Keywords::OR);
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::test(){
    // look for leading parentheses expression
    if (this->la(1) == T_DELIMITER && this->lt(1).getText() == "("){
        ParenthesesNode * pn = this->parentheses();
        if (this->lt(1).getText() == "<<"){
            this->shiftLeft(pn);
        }
        else if (this->lt(1).getText() == ">>"){
            this->shiftRight(pn);
        }
        else if (this->lt(1).getText() == "&"){
            this->bitAnd(pn);
        }
        else if (this->lt(1).getText() == "|"){
            this->bitOr(pn);
        }
        else if (this->lt(1).getText() == "^"){
            this->bitXor(pn);        
        }
        else if (this->lt(1).getText() == "<"){
            this->lessThan(pn);
        }
        else if (this->lt(1).getText() == "<="){
            this->lessThanOrEqual(pn);
        }
        else if (this->lt(1).getText() == ">"){
            this->greaterThan(pn);
        } 
        else if (this->lt(1).getText() == ">="){
            this->greaterThanOrEqual(pn);
        }
        else if (this->lt(1).getText() == "=="){
            this->equalTo(pn);
        }
        else if (this->lt(1).getText() == "!="){
            this->notEqualTo(pn);
        }
        else if (this->lt(1).getText() == Keywords::AND){
            this->boolAnd(pn);
        }
        else if (this->lt(1).getText() == Keywords::OR){
            this->boolOr(pn);
        }
        else {
            this->binaryOperation(pn);
        }
        return;
    }

    // no leading parentheses
    if (this->speculateShiftLeft()){
        this->shiftLeft();
    }
    else if (this->speculateShiftRight()){
        this->shiftRight();
    }
    else if (this->speculateBitAnd()){
        this->bitAnd();
    }
    else if (this->speculateBitOr()){
        this->bitOr();
    }
    else if (this->speculateBitXor()){
        this->bitXor();        
    }
    else if (this->speculateLessThan()){
        this->lessThan();
    }
    else if (this->speculateLessThanOrEqual()){
        this->lessThanOrEqual();
    }
    else if (this->speculateGreaterThan()){
        this->greaterThan();
    } 
    else if (this->speculateGreaterThanOrEqual()){
        this->greaterThanOrEqual();
    }
    else if (this->speculateEqualTo()){
        this->equalTo();
    }
    else if (this->speculateNotEqualTo()){
        this->notEqualTo();
    }
    else if (this->speculateBoolAnd()){
        this->boolAnd();
    }
    else if (this->speculateBoolOr()){
        this->boolOr();
    }
    else {
        this->binaryOperation();
    }
};

bool BrioParser::speculateLessThan(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(lessThanMemo)) return success;

    mark();
    try{
        this->lessThan();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(lessThanMemo, startIndex, !success);
    return success;
}

void BrioParser::lessThan(){
    BoolLessThanNode* node = new BoolLessThanNode(this->next("<"));
    this->addNodeAndDescend(node);

    // match less than bool operation rules
    this->term();
    this->match(T_OPERATOR, "<");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::lessThan(ParenthesesNode * lhs){
    BoolLessThanNode* node = new BoolLessThanNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match less than bool operation rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "<");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateLessThanOrEqual(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(lessThanOrEqualMemo)) return success;

    mark();
    try{
        this->lessThanOrEqual();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(lessThanOrEqualMemo, startIndex, !success);
    return success;
}

void BrioParser::lessThanOrEqual(){
    BoolLessThanOrEqualNode* node = new BoolLessThanOrEqualNode(this->next("<="));
    this->addNodeAndDescend(node);

    // match less than or equal bool operation rules
    this->term();
    this->match(T_OPERATOR, "<=");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::lessThanOrEqual(ParenthesesNode * lhs){
    BoolLessThanOrEqualNode* node = new BoolLessThanOrEqualNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match less than or equal bool operation rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "<=");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateEqualTo(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(equalToMemo)) return success;

    mark();
    try{
        this->equalTo();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(equalToMemo, startIndex, !success);
    return success;
}

void BrioParser::equalTo(){
    BoolEqualsNode* node = new BoolEqualsNode(this->next("=="));
    this->addNodeAndDescend(node);

    // match equals bool operation rules
    this->term();
    this->match(T_OPERATOR, "==");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::equalTo(ParenthesesNode * lhs){
    BoolEqualsNode* node = new BoolEqualsNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match equals bool operation rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "==");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateNotEqualTo(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(notEqualToMemo)) return success;

    mark();
    try{
        this->notEqualTo();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(notEqualToMemo, startIndex, !success);
    return success;
}

void BrioParser::notEqualTo(){
    BoolNotEqualsNode* node = new BoolNotEqualsNode(this->next("!="));
    this->addNodeAndDescend(node);

    // match not equals bool operation rules
    this->term();
    this->match(T_OPERATOR, "!=");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::notEqualTo(ParenthesesNode * lhs){
    BoolNotEqualsNode* node = new BoolNotEqualsNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match not equals bool operation rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "!=");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateGreaterThan(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(greaterThanMemo)) return success;

    mark();
    try{
        this->greaterThan();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(greaterThanMemo, startIndex, !success);
    return success;
}

void BrioParser::greaterThan(){
    BoolGreaterThanNode* node = new BoolGreaterThanNode(this->next(">"));
    this->addNodeAndDescend(node);

    // match greater than bool operation rules
    this->term();
    this->match(T_OPERATOR, ">");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::greaterThan(ParenthesesNode * lhs){
    BoolGreaterThanNode* node = new BoolGreaterThanNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match greater than bool operation rules
    this->addChild(lhs);
    this->match(T_OPERATOR, ">");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateGreaterThanOrEqual(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(greaterThanOrEqualMemo)) return success;

    mark();
    try{
        this->greaterThanOrEqual();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(greaterThanOrEqualMemo, startIndex, !success);
    return success;
}

void BrioParser::greaterThanOrEqual(){
    BoolGreaterThanOrEqualNode* node = new BoolGreaterThanOrEqualNode(this->next(">="));
    this->addNodeAndDescend(node);

    // match greater than or equal bool operation rules
    this->term();
    this->match(T_OPERATOR, ">=");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::greaterThanOrEqual(ParenthesesNode * lhs){
    BoolGreaterThanOrEqualNode* node = new BoolGreaterThanOrEqualNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match greater than or equal bool operation rules
    this->addChild(lhs);
    this->match(T_OPERATOR, ">=");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateShiftLeft(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(shiftLeftMemo)) return success;

    mark();
    try{
        this->shiftLeft();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(shiftLeftMemo, startIndex, !success);
    return success;
}

void BrioParser::shiftLeft(){
    BitShiftLeftNode* node = new BitShiftLeftNode(this->next("<<"));
    this->addNodeAndDescend(node);

    // match bit shift left rules
    this->term();
    this->match(T_OPERATOR, "<<");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::shiftLeft(ParenthesesNode * lhs){
    BitShiftLeftNode* node = new BitShiftLeftNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match bit shift left rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "<<");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateShiftRight(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(shiftRightMemo)) return success;

    mark();
    try{
        this->shiftRight();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(shiftRightMemo, startIndex, !success);
    return success;
}

void BrioParser::shiftRight(){
    BitShiftRightNode* node = new BitShiftRightNode(this->next(">>"));
    this->addNodeAndDescend(node);

    // match bit shift right rules
    this->term();
    this->match(T_OPERATOR, ">>");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::shiftRight(ParenthesesNode * lhs){
    BitShiftRightNode* node = new BitShiftRightNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match bit shift right rules
    this->addChild(lhs);
    this->match(T_OPERATOR, ">>");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::binaryOperation(ParenthesesNode * pn){
    if (this->lt(1).getText() == "+"){
        this->addition(pn);
    }
    else if (this->lt(1).getText() == "-"){
        this->subtraction(pn);
    }
    else if (this->lt(1).getText() == "*"){
        this->multiplication(pn);
    }
    else if (this->lt(1).getText() == "/"){
        this->division(pn);
    }
    else if (this->lt(1).getText() == "%"){
        this->modulus(pn);
    }
    else if (this->lt(1).getText() == "**"){
        this->exponent(pn);
    }else{
        // we parsed a parentheses but no subsequent operator
        this->addChild(pn);
    }
    return;
};

void BrioParser::binaryOperation(){
    if (this->speculateAddition()){
        this->addition();
    }
    else if (this->speculateSubtraction()){
        this->subtraction();
    }
    else if (this->speculateMultiplication()){
        this->multiplication();
    }
    else if (this->speculateDivision()){
        this->division();
    }
    else if (this->speculateModulus()){
        this->modulus();
    }
    else if (this->speculateExponent()){
        this->exponent();
    }
    else {
        this->term();
    }
};

bool BrioParser::speculateBitOr(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(bitOrMemo)) return success;

    mark();
    try{
        this->bitOr();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(bitOrMemo, startIndex, !success);
    return success;
}

void BrioParser::bitOr(){
    BitOrExpressionNode* node = new BitOrExpressionNode(this->next("|"));
    this->addNodeAndDescend(node);

    // match bitwise or rules
    this->term();
    this->match(T_OPERATOR, "|");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::bitOr(ParenthesesNode * lhs){
    BitOrExpressionNode* node = new BitOrExpressionNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "|");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateBitOrAssign(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(bitOrAssignMemo)) return success;

    mark();
    try{
        this->bitOrAssign();
    }
    catch(RecognitionException e){success=false;}
    release();

    if (isSpeculating()) memoize(bitOrAssignMemo, startIndex, !success);
    return success;
};

void BrioParser::bitOrAssign(){
    BitOrAssignNode* node = new BitOrAssignNode(this->lt(2));
    this->addNodeAndDescend(node);

    // match subtraction rules
    this->term();
    this->match(T_OPERATOR, "|=");
    this->expression();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateBitXor(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(bitXorMemo)) return success;

    mark();
    try{
        this->bitXor();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(bitXorMemo, startIndex, !success);
    return success;
}

void BrioParser::bitXor(){
    BitXorExpressionNode* node = new BitXorExpressionNode(this->next("^"));
    this->addNodeAndDescend(node);

    // match bitwise xor rules
    this->term();
    this->match(T_OPERATOR, "^");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::bitXor(ParenthesesNode * lhs){
    BitXorExpressionNode* node = new BitXorExpressionNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "^");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateBitAnd(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(bitAndMemo)) return success;

    mark();
    try{
        this->bitAnd();
    }
    catch(RecognitionException ex){success=false;}
    release();

    if (isSpeculating()) memoize(bitAndMemo, startIndex, !success);
    return success;
}

void BrioParser::bitAnd(){
    BitAndExpressionNode* node = new BitAndExpressionNode(this->next("&"));
    this->addNodeAndDescend(node);

    // match bitwise and rules
    this->term();
    this->match(T_OPERATOR, "&");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::bitAnd(ParenthesesNode * lhs){
    BitAndExpressionNode* node = new BitAndExpressionNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "&");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateBitAndAssign(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(bitAndAssignMemo)) return success;

    mark();
    try{
        this->bitAndAssign();
    }
    catch(RecognitionException e){success=false;}
    release();

    if (isSpeculating()) memoize(bitAndAssignMemo, startIndex, !success);
    return success;
};

void BrioParser::bitAndAssign(){
    BitAndAssignNode* node = new BitAndAssignNode(this->lt(2));
    this->addNodeAndDescend(node);

    // match subtraction rules
    this->term();
    this->match(T_OPERATOR, "&=");
    this->expression();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateExponent(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(exponentMemo)) return success;

    mark();
    try{
        this->exponent();
    }
    catch(RecognitionException e){success = false;};
    release();

    if (isSpeculating()) memoize(exponentMemo, startIndex, !success);
    return success;
}

void BrioParser::exponent(){
    ExponentExpressionNode* node = new ExponentExpressionNode(this->next("**"));
    this->addNodeAndDescend(node);

    // match addition rules
    this->term();
    this->match(T_OPERATOR, "**");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::exponent(ParenthesesNode * lhs){
    ExponentExpressionNode* node = new ExponentExpressionNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "**");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateModulus(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(modulusMemo)) return success;

    mark();
    try{
        this->modulus();
    }
    catch(RecognitionException e){success = false;};
    release();

    if (isSpeculating()) memoize(modulusMemo, startIndex, !success);
    return success;
}

void BrioParser::modulus(){
    ModulusExpressionNode* node = new ModulusExpressionNode(this->next("%"));
    this->addNodeAndDescend(node);

    // match addition rules
    this->term();
    this->match(T_OPERATOR, "%");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::modulus(ParenthesesNode * lhs){
    ModulusExpressionNode* node = new ModulusExpressionNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "%");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateAddition(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(additionMemo)) return success;

    mark();
    try{
        this->addition();
    }
    catch(RecognitionException e){success = false;};
    release();

    if (isSpeculating()) memoize(additionMemo, startIndex, !success);
    return success;
}

void BrioParser::addition(){
    AddExpressionNode* node = new AddExpressionNode(this->next("+"));
    this->addNodeAndDescend(node);

    // match addition rules
    this->term();
    this->match(T_OPERATOR, "+");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::addition(ParenthesesNode * lhs){
    AddExpressionNode* node = new AddExpressionNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "+");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateAdditionAssign(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(additionAssignMemo)) return success;

    mark();
    try{
        this->additionAssign();
    }
    catch(RecognitionException e){success=false;}
    release();

    if (isSpeculating()) memoize(additionAssignMemo, startIndex, !success);
    return success;
};

void BrioParser::additionAssign(){
    AdditionAssignNode* node = new AdditionAssignNode(this->lt(2));
    this->addNodeAndDescend(node);

    // match subtraction rules
    this->term();
    this->match(T_OPERATOR, "+=");
    this->expression();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateSubtraction(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(subtractionMemo)) return success;

    mark();
    try{
        this->subtraction();
    }
    catch(RecognitionException e){success = false;};
    release();

    if (isSpeculating()) memoize(subtractionMemo, startIndex, !success);
    return success;
}

void BrioParser::subtraction(){
    SubtractExpressionNode* node = new SubtractExpressionNode(this->next("-"));
    this->addNodeAndDescend(node);

    // match subtraction rules
    this->term();
    this->match(T_OPERATOR, "-");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::subtraction(ParenthesesNode * lhs){
    SubtractExpressionNode* node = new SubtractExpressionNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "-");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateSubtractionAssign(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(subtractionAssignMemo)) return success;

    mark();
    try{
        this->subtractionAssign();
    }
    catch(RecognitionException e){success=false;}
    release();

    if (isSpeculating()) memoize(subtractionAssignMemo, startIndex, !success);
    return success;
};

void BrioParser::subtractionAssign(){
    SubtractionAssignNode* node = new SubtractionAssignNode(this->lt(2));
    this->addNodeAndDescend(node);

    // match subtraction rules
    this->term();
    this->match(T_OPERATOR, "-=");
    this->expression();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateMultiplication(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(multiplicationMemo)) return success;
    
    mark();
    try{
        this->multiplication();
    }
    catch(RecognitionException e){success = false;};
    release();

    if (isSpeculating()) memoize(multiplicationMemo, startIndex, !success);
    return success;
}

void BrioParser::multiplication(){   
    MultiplyExpressionNode* node = new MultiplyExpressionNode(this->next("*"));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->term();
    this->match(T_OPERATOR, "*");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::multiplication(ParenthesesNode * lhs){
    MultiplyExpressionNode* node = new MultiplyExpressionNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "*");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateMultiplicationAssign(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(multiplicationAssignMemo)) return success;

    mark();
    try{
        this->multiplicationAssign();
    }
    catch(RecognitionException e){success=false;}
    release();

    if (isSpeculating()) memoize(multiplicationAssignMemo, startIndex, !success);
    return success;
};

void BrioParser::multiplicationAssign(){
    MultiplicationAssignNode* node = new MultiplicationAssignNode(this->lt(2));
    this->addNodeAndDescend(node);

    // match subtraction rules
    this->term();
    this->match(T_OPERATOR, "*=");
    this->expression();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateDivision(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(divisionMemo)) return success;

    mark();
    try{
        this->division();
    }
    catch(RecognitionException e){success = false;};
    release();

    if (isSpeculating()) memoize(divisionMemo, startIndex, !success);
    return success;
}

void BrioParser::division(){
    DivideExpressionNode* node = new DivideExpressionNode(this->next("/"));
    this->addNodeAndDescend(node);

    // match division rules
    this->term();
    this->match(T_OPERATOR, "/");
    this->test();

    // restore node position
    this->restoreNode();
};

void BrioParser::division(ParenthesesNode * lhs){
    DivideExpressionNode* node = new DivideExpressionNode(this->lt(1));
    this->addNodeAndDescend(node);

    // match multiplication rules
    this->addChild(lhs);
    this->match(T_OPERATOR, "/");
    this->test();

    // restore node position
    this->restoreNode();
};

bool BrioParser::speculateDivisionAssign(){
    bool success = true;
    int startIndex = this->pIndex();
    if (isSpeculating() && alreadyParsedRule(divisionAssignMemo)) return success;

    mark();
    try{
        this->divisionAssign();
    }
    catch(RecognitionException e){success=false;}
    release();

    if (isSpeculating()) memoize(divisionAssignMemo, startIndex, !success);
    return success;
};

void BrioParser::divisionAssign(){
    DivisionAssignNode* node = new DivisionAssignNode(this->lt(2));
    this->addNodeAndDescend(node);

    // match subtraction rules
    this->term();
    this->match(T_OPERATOR, "/=");
    this->expression();
    this->optEndStatement();

    // restore node position
    this->restoreNode();
};

void BrioParser::term(){
    if (this->speculateMethodCall()){
        this->methodCall();
    }
    else if (this->speculateMemberAccess()){
        this->memberAccess();
    }
    else if (this->la(1) == T_IDENTIFIER){
        this->identifier();
    }
    else if (this->la(1) == T_LITERAL_INT){
        this->literalInt();
    }
    else if (this->lt(1).getText() == "-" && this->la(2) == T_LITERAL_INT){
        this->literalIntNegative();
    }
    else if (this->la(1) == T_LITERAL_DECIMAL){
        this->literalDecimal();
    }
    else if (this->lt(1).getText() == "-" && this->la(2) == T_LITERAL_DECIMAL){
        this->literalDecimalNegative();
    }
    else if (this->la(1) == T_LITERAL_STRING){
        this->literalString();
    }
    else if (this->la(1) == T_LITERAL_BOOL){
        this->literalBoolean();
    }
    else if (this->la(1) == T_LITERAL_NONE){
        this->literalNone();
    }
    else if (this->la(1) == T_DELIMITER && this->lt(1).getText() == "["){
        this->array();
    }
    else if (this->la(1) == T_DELIMITER && this->lt(1).getText() == "{"){
        this->dictionary();
    }
    else if (this->la(1) == T_DELIMITER && this->lt(1).getText() == "("){
        this->parentheses();
    }
    else{
        int lineno = this->lt(1).getLineNo();
        throw SyntaxError("invalid term: '" + this->lt(1).getText() + "'", lineno);
    }
};

void BrioParser::statement(){
    if (this->la(1) == T_KEYWORD){
        if (this->speculateMethodDeclaration()){
            this->methodDeclaration();
        }
        else if (this->speculateClassDeclaration()){
            this->classDeclaration();
        }
        else if (this->lt(1).getText() == Keywords::LET && this->la(2) == T_IDENTIFIER){
            this->variableDeclaration();
        }
        else if (this->lt(1).getText() == Keywords::RETURN){
            this->returnValue();
        }
        else if (this->lt(1).getText() == Keywords::IF && this->la(2) == T_DELIMITER){
            this->ifStatement();
        }
        else if (this->lt(1).getText() == Keywords::TRY && this->la(2) == T_DELIMITER){
            this->tryStatement();
        }
        else if (this->lt(1).getText() == Keywords::RAISE){
            this->raiseStatement();
        }
        else if (this->lt(1).getText() == Keywords::WHILE && this->la(2) == T_DELIMITER){
            this->whileStatement();
        }
        else if (this->lt(1).getText() == Keywords::FOR && this->la(2) == T_DELIMITER){
            this->forStatement();
        }
        else if (this->lt(1).getText() == Keywords::EACH && this->la(2) == T_DELIMITER){
            this->eachStatement();
        }
        else if (this->lt(1).getText() == Keywords::SKIP){
            this->skipStatement();
        }
        else if (this->lt(1).getText() == Keywords::IMPORT){
            this->import();
        }
        else if (this->lt(1).getText() == Keywords::FROM){
            this->importFrom();
        }
        else {
            int lineno = this->lt(1).getLineNo();
            throw SyntaxError("unsupported statement", lineno);
        }
    }
    else {
        this->expression();
    }
};

