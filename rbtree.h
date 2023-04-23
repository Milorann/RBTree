
#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>

enum class Color { RED, BLACK };

template <typename T>
struct Node {
    T key;
    Node *left;
    Node *right;
    Node *parent;
    Color color;

    Node(T key);

    Node(T key, Node *left, Node *right, Node *parent, Color color);
};

template <typename T>
class RBTree {
public:
    RBTree();

    RBTree(std::initializer_list<T> list);

    ~RBTree();

    void insert(T key);

    int size() const;

    bool empty() const;

    T *lowerBound(T key) const;

    T *find(T key) const;

    Node<T> *root{};

private:
    int size_;

    void nodeDestructor(Node<T> *node);

    void fixTreeInsert(Node<T> *node);

    Node<T> *findUncle(Node<T> *node);

    Node<T> *findGrandparent(Node<T> *node);

    void leftRotation(Node<T> *node);

    void rightRotation(Node<T> *node);
};
#endif // RBTREE_H
