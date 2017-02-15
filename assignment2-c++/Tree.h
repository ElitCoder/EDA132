#ifndef TREE_H
#define TREE_H

#include <vector>
#include <string>
#include <array>

struct Node {
    std::vector<Node*> nodes;
    
    std::string value;
};

class Tree {
public:
    Tree(const std::string &root);
    Tree(const Tree &t);
    ~Tree();
    
    void add(const std::string &leaf, const Tree &sub);
    bool isLeaf(const Node *node) const;
    
    friend std::ostream& operator<<(std::ostream& out, const Tree &t);
    
private:
    void clear(Node *root);
    void copy(Node *from, Node **to);
    void print(const Node *node, int depth) const;
    
    Node *m_root;
};

#endif