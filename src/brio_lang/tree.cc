///
/// Brio Lang
/// tree.cc
///
/// Normalized Heterogeneous AST
///

#include <brio_lang/tree.h>
#include <brio_lang/token.h>
#include <iostream>

using namespace std;

AST::AST() { 
    token = Token(); 
};

AST::AST(Token node_token){
    token = node_token;
};

bool AST::isNil(){
    return this->token.getType() == -1;
};

int AST::getLineNo(){
    return this->token.getLineNo();
}

void AST::addChild(AST * node){
    node->parent = this;
    this->children.push_back(node);
    node->index = size(this->children) - 1;
};

AST * AST::popChild(){
    if (size(this->children) == 0) return nullptr;
    AST * lastChild = this->children[size(this->children)-1];
    this->children.pop_back();
    lastChild->parent = nullptr;
    return lastChild;
};

void AST::replace(AST * node){
    node->parent = this->parent;
    this->parent->children[this->index] = node;
    delete this;
};

int AST::getChildCount(){
    return this->children.size();
};

Token AST::getToken(){
    return this->token;
}

int AST::getTokenType(){
    return this->token.getType();
};

int AST::getTokenIndex(){
    return this->token.getIndex();
};

string AST::getNodeText(){
    return this->token.getText();
};

int AST::getBindingPower(){
    return this->token.getBindingPower();
}

AST * AST::clone(){
    return new AST(*this);
}

AST * AST::getSibling(int index){
    if (this->parent == nullptr) return nullptr;
    return this->parent->children[index];
};

AST * AST::getAncestor(string node_type){
    AST * node = this->parent;
    while (node->getType() != ProgramNode::type){
        if (node->getType() == node_type){
            return node;
        }
        node = node->parent;
    }
    return nullptr;
};

bool AST::hasAncestor(string node_type){
    return this->getAncestor(node_type) != nullptr;
};

bool AST::hasAncestorExpr(){
    AST * node = this->parent;
    while (node->getType() != ProgramNode::type){
        if (dynamic_cast<BinExpressionNode*>(node) != nullptr){
           return true;
        }
        else if (dynamic_cast<BoolExpressionNode*>(node) != nullptr){
           return true;
        }
        else if (dynamic_cast<BitExpressionNode*>(node) != nullptr){
           return true;
        }
        node = node->parent;
    }
    return false;
};

ProgramNode::ProgramNode() : 
    AST() { };

VarDeclarationNode::VarDeclarationNode(Token token) : 
    AST(token) { };

VarAssignmentNode::VarAssignmentNode(Token token) : 
    AST(token) { };

AdditionAssignNode::AdditionAssignNode(Token token) : 
    AST(token) { };

SubtractionAssignNode::SubtractionAssignNode(Token token) : 
    AST(token) { };

MultiplicationAssignNode::MultiplicationAssignNode(Token token) : 
    AST(token) { };

DivisionAssignNode::DivisionAssignNode(Token token) : 
    AST(token) { };

BitOrAssignNode::BitOrAssignNode(Token token) : 
    AST(token) { };

BitAndAssignNode::BitAndAssignNode(Token token) : 
    AST(token) { };

ClassDeclarationNode::ClassDeclarationNode(Token token) : 
    AST(token) { };

ClassInstantiationNode::ClassInstantiationNode(Token token) : 
    AST(token) { };

MethodDeclarationNode::MethodDeclarationNode(Token token) : 
    AST(token) { };

MethodCallNode::MethodCallNode(Token token) : 
    AST(token) { };

ModifierNode::ModifierNode(Token token) : 
    AST(token) { };

ParamsListNode::ParamsListNode(Token token) : 
    AST(token) { };

ParenthesesNode::ParenthesesNode(Token token) : 
    AST(token) { };

BinExpressionNode::BinExpressionNode(Token token) : 
    AST(token) { };

BoolExpressionNode::BoolExpressionNode(Token token) : 
    AST(token) { };

BitExpressionNode::BitExpressionNode(Token token) : 
    AST(token) { };

BitOrExpressionNode::BitOrExpressionNode(Token token):
    BitExpressionNode(token) { };

BitAndExpressionNode::BitAndExpressionNode(Token token):
    BitExpressionNode(token) { };

BitXorExpressionNode::BitXorExpressionNode(Token token):
    BitExpressionNode(token) { };

ModulusExpressionNode::ModulusExpressionNode(Token token):
    BinExpressionNode(token) { };

ExponentExpressionNode::ExponentExpressionNode(Token token):
    BinExpressionNode(token) { };

AddExpressionNode::AddExpressionNode(Token token) : 
    BinExpressionNode(token) { };

SubtractExpressionNode::SubtractExpressionNode(Token token) : 
    BinExpressionNode(token) { };

MultiplyExpressionNode::MultiplyExpressionNode(Token token) : 
    BinExpressionNode(token) { };

DivideExpressionNode::DivideExpressionNode(Token token) : 
    BinExpressionNode(token) { };

IfStatementNode::IfStatementNode(Token token) :
    AST(token) { };

IfConditionBlockNode::IfConditionBlockNode(Token token) :
    AST(token) { };

ImportNode::ImportNode(Token token) :
    AST(token) { };

ImportedModuleNode::ImportedModuleNode(Token token) :
    AST(token) { };

FromImportNode::FromImportNode(Token token) :
    AST(token) { };

ForStatementNode::ForStatementNode(Token token) :
    AST(token) { };

EachStatementNode::EachStatementNode(Token token) :
    AST(token) { };

WhileStatementNode::WhileStatementNode(Token token) :
    AST(token) { };

SkipStatementNode::SkipStatementNode(Token token) :
    AST(token) { };

TryStatementNode::TryStatementNode(Token token) :
    AST(token) { };

CatchStatementNode::CatchStatementNode(Token token) :
    AST(token) { };

RaiseStatementNode::RaiseStatementNode(Token token) :
    AST(token) { };

AsNode::AsNode(Token token) :
    AST(token) { };

MemberAccessNode::MemberAccessNode(Token token) : 
    AST(token) { };

IndexNode::IndexNode(Token token) : 
    AST(token) { };

BlockNode::BlockNode(Token token) : 
    AST(token) { };

SuperClassNode::SuperClassNode(Token token) :
    AST(token) {};

LiteralStringNode::LiteralStringNode(Token token) : 
    AST(token) { };

LiteralIntNode::LiteralIntNode(Token token) : 
    AST(token) { };

LiteralDecimalNode::LiteralDecimalNode(Token token) : 
    AST(token) { };

LiteralBooleanNode::LiteralBooleanNode(Token token) : 
    AST(token) { };

LiteralNoneNode::LiteralNoneNode(Token token) : 
    AST(token) { };

ArrayNode::ArrayNode(Token token) : 
    AST(token) { };

DictionaryNode::DictionaryNode(Token token) : 
    AST(token) { };

DictionaryKeyValueNode::DictionaryKeyValueNode(Token token) : 
    AST(token) { };

ReturnNode::ReturnNode(Token token) : 
    AST(token) { };

IdentifierNode::IdentifierNode(Token token) : 
    AST(token) { };

BoolOrNode::BoolOrNode(Token token) :
    BoolExpressionNode(token) { };

BoolAndNode::BoolAndNode(Token token) :
    BoolExpressionNode(token) { };

BoolEqualsNode::BoolEqualsNode(Token token) : 
    BoolExpressionNode(token) { };

BoolNotEqualsNode::BoolNotEqualsNode(Token token) : 
    BoolExpressionNode(token) { };

BoolGreaterThanNode::BoolGreaterThanNode(Token token) : 
    BoolExpressionNode(token) { };

BoolGreaterThanOrEqualNode::BoolGreaterThanOrEqualNode(Token token) : 
    BoolExpressionNode(token) { };

BoolLessThanNode::BoolLessThanNode(Token token) : 
    BoolExpressionNode(token) { };

BoolLessThanOrEqualNode::BoolLessThanOrEqualNode(Token token) : 
    BoolExpressionNode(token) { };

BitShiftLeftNode::BitShiftLeftNode(Token token) : 
    BitExpressionNode(token) { };

BitShiftRightNode::BitShiftRightNode(Token token) : 
    BitExpressionNode(token) { };