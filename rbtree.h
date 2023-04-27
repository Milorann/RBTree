#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>

enum class Color {
    RED, BLACK
};

struct Node {
    int key;
    Node *left;
    Node *right;
    Node *parent;
    Color color;

    explicit Node(int key);

    Node(int key, Node *left, Node *right, Node *parent, Color color);

    Node *sibling();
};

class RBTree {
public:
    RBTree();

    RBTree(std::initializer_list<int> list);

    ~RBTree();

    void insert(int key);

    [[nodiscard]] int size() const;

    [[nodiscard]] bool empty() const;

    int *lowerBound(int key) const;

    int *upperBound(int key) const;

    [[nodiscard]] int *find(int key) const;

    void erase(const int &key);

    Node *root{};

private:
    int size_;

    void nodeDestructor(Node *node);

    void fixTreeInsert(Node *node);

    Node *findUncle(Node *node);

    Node *findGrandparent(Node *node);

    void leftRotation(Node *node);

    void rightRotation(Node *node);

    void eraseNode(Node *node);

    Node *findReplacementNode(Node *node);

    void repairDoubleBlack(Node *node);

    Node *findNode(int key);
};

#endif // RBTREE_H
