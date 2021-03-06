///
/// Brio Lang
/// visitor.cc
///
/// External Tree Visitors
///

#include <brio_lang/visitor.h>
#include <brio_lang/tree.h>


/**
 * Example visitor that prints the type and children count of each node.
 */
void IndependentPrintVisitor::visit(AST * node){
    this->visit(node, 0);
};

void IndependentPrintVisitor::visit(AST * node, int depth){
    std::string prefix = "";
    for (int i = 0; i < depth; i++){ prefix += "--"; };
    std::cout << prefix << "(" << node->getType() << ")  children=" << node->children.size() << std::endl;
    for (int i=0; i < node->children.size(); i++){
        this->visit(node->children[i], depth+1);
    }
};

/**
 * Helper method to return a pointer to the main method node.
 */
AST * getMainMethod(ProgramNode * node){
    for (int i=0; i < node->children.size(); i++){
        AST * child = node->children[i];
        if (child->getType() == MethodDeclarationNode::type){
            AST * identifier = child->children[0];
            if (identifier->getNodeText() == "main"){
                return child;
            }
        }
    }
    return NULL;
};

/**
 * Visitor that will look for the "main" method, and print the first
 * value for each "print(...) method call.
 * NOTE: This is for dev / testing purposes only!
 */
void InvokeMainMethodPrint::visit(ProgramNode * node){
    AST * main_method = getMainMethod(node);

    // look for print
    AST * main_block = main_method->children[2];
    for (int z=0; z < main_block->children.size(); z++){
        AST * block_elem = main_block->children[z];
        
        // look for method call
        if (block_elem->getType() == MethodCallNode::type){
            string method_name = block_elem->children[0]->getNodeText();
            if (method_name == "print"){

                // get print params
                AST * params = block_elem->children[1];

                for (int z=0; z<params->children.size(); z++){
                    AST * literal = params->children[z];
                    // output value
                    std::cout << literal->getNodeText() << " ";
                }
                std::cout << std::endl;
            }
        }
    }
};

/** Top-down visitor that walks through the AST and re-writes nodes based 
 * on operator precedence / the binding power of operators.
 */
void OperatorPrecedenceVisitor::visit(AST * node){
    AST * nextNode = node;

    if (node->getTokenType() == T_OPERATOR){
        AST * rhs = node->children[1];

        if (rhs->getTokenType() == T_OPERATOR){
            // determine if right child node has lower or equal precedence
            if (rhs->getBindingPower() <= node->getBindingPower()){
                AST * nodeCp = node->clone();
                int nodeParentChildCount = size(node->parent->children);
                node->parent->children[nodeParentChildCount-1] = rhs;
                rhs->parent = node->parent;
                nodeCp->children[1] = rhs->children[0];
                nodeCp->children[1]->parent = nodeCp;
                nodeCp->parent = rhs;
                rhs->children[0] = nodeCp;
                nextNode = rhs->parent->parent;
                delete node;
            }
        }
    }

    for (int i=0; i < nextNode->children.size(); i++){
        this->visit(nextNode->children[i]);
    }
};