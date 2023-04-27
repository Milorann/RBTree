#ifndef RBTREE_H
#define RBTREE_H

#include "Observer.h"

#include <iostream>
#include <functional>

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
    struct DrawData
    {
        Node *root;
        enum class Status { DEFAULT, FOUND, PASSING, DELETED };
        std::pair<Node *, Status> changedNode;
    };

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

    void subscribe(Observer<DrawData> *observer);

private:
    Observable<DrawData> drawData_ = DrawData{root};

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
