#include "Tree.h"

#include <iostream>

using namespace std;

Tree::Tree(const string &root) {
    m_root = new Node;
    
    m_root->value = root;
}

void Tree::copy(Node *from, Node **to) {
    (*to)->value = from->value;
    
    for(Node *node : from->nodes) {
        (*to)->nodes.push_back(new Node);
        
        copy(node, &(*to)->nodes.back());
    }
}

void Tree::print(const Node *node, int depth) const {
    if(node == nullptr) {
        return;
    }
    
    for(int i = 0; i < depth; i++) {
        cout << "  ";
    }
    
    cout << "Node: " << node->value << '\n';
    
    for(const Node *leaf : node->nodes) {        
        print(leaf, depth + 1);
    }
}

Tree::Tree(const Tree &t) {
    if(t.m_root == nullptr || this == &t) {
        m_root = nullptr;
        
        return;
    }
    
    m_root = new Node;

    copy(t.m_root, &m_root);
}

void Tree::clear(Node *root) {
    if(root == nullptr) {
        return;
    }
          
    for(Node *node : root->nodes) {
        clear(node);
    }
    
    delete root;
}

Tree::~Tree() {
    clear(m_root);
}

void Tree::add(const std::string &leaf, const Tree &sub) {
    Node *node = new Node;
    
    node->value = leaf;
    m_root->nodes.push_back(node);
    
    if(sub.m_root == nullptr) {
        return;
    }
    
    Node *branch = new Node;
    
    m_root->nodes.back()->nodes.push_back(branch);
    
    copy(sub.m_root, &branch);
}

ostream& operator<<(ostream& out, const Tree &t) {
    t.print(t.m_root, 0);
    
    return out;
}