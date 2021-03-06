///
/// Brio Lang
/// dot.cc
///

#include <brio_lang/dot.h>


DotNode::DotNode(AST * n){
    node = n;
};

DotGraphVisitor::DotGraphVisitor(){};

/**
 * Returns a string representation of an AST using DOT.
 */
string DotGraphVisitor::generate(AST * node){
    return this->getHeader() + this->getBody(node) + this->getFooter();
};

/**
 * Returns the string header of the DOT representation.
 */
string DotGraphVisitor::getHeader(){
    return "graph astgraph {\n"
           "node [fontsize=12, fontname=\"Helvetica\"];\n";
};

/**
 * Returns the string body of the DOT representation.
 */
string DotGraphVisitor::getBody(AST * node){
    string dot_body;
    n_count = 1;
    DotNode * d_node = new DotNode(node);
    d_node->num = n_count;
    queue.push_back(d_node);
    
    // append root node definition
    string root_def = "node" + std::to_string(n_count) + " [label=\"" + d_node->node->getType() + "\"];\n";
    dot_body.append(root_def);
    n_count += 1;

    // append child node definitions
    while (queue.size() > 0){
        DotNode * n = queue[0];
        queue.pop_front();

        for (int i=0; i < n->node->children.size(); i++){
            DotNode * d_child = new DotNode(n->node->children[i]);
            d_child->num = n_count;

            string node_def = "node" + std::to_string(n_count) + " [label=\"" + d_child->node->getType() + "\n'" + d_child->node->getNodeText() + "'\"];\n";
            dot_body.append(node_def);
            
            string link = "node" + std::to_string(n->num) + " -- " + "node" + std::to_string(d_child->num) + "\n";
            dot_body.append(link);

            n_count += 1;
            queue.push_back(d_child);
        };
    };

    return dot_body;
}

/**
 * Returns the string footer of the DOT representation.
 */
string DotGraphVisitor::getFooter(){
    return "}";
};