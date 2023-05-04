#ifndef RBTREE_H
#define RBTREE_H

#include "Observer.h"

#include <functional>
#include <unistd.h>

namespace rbtree {
enum class Color { Red, Black };

struct Node
{
    int key;
    Color color;
    Node *left;
    Node *right;
    Node *parent;
};

class RBTree
{
public:
    struct DrawData
    {
        enum class Status { Default, Found, Passing, Deleted };

        Node *root;
        std::pair<const Node *, Status> changedNode;
    };

    RBTree() = default;
    RBTree(std::initializer_list<int> list);
    RBTree(const RBTree &) = delete;
    RBTree &operator=(const RBTree &) = delete;
    RBTree(RBTree &&) noexcept = delete;
    RBTree &operator=(RBTree &&) noexcept = delete;
    ~RBTree();

    void insert(int key);

    [[nodiscard]] int size() const;
    [[nodiscard]] bool empty() const;

    int lowerBound(int key);
    int upperBound(int key);
    int find(int key);

    void erase(int key);

    void clear();

    void subscribe(Observer<DrawData> *observer);

    const Node *root() const;

private:
    void destroySubtree(Node *node);

    void rebalanceAfterInsert(Node *node);

    Node *findSibling(Node *node);
    Node *findUncle(Node *node);
    Node *findGrandparent(Node *node);
    Node *findNode(int key);
    Node *findInsertionParent(int key);
    Node *findReplacementNode(Node *node);

    void leftRotation(Node *node);
    void rightRotation(Node *node);

    void eraseNode(Node *node);
    void rebalanceAfterDeletion(Node *node);

    Node *root_ = nullptr;
    int size_ = 0;

    Observable<DrawData> drawPort_ = DrawData{root_};
};
} // namespace rbtree
#endif // RBTREE_H
