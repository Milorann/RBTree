#ifndef RBTREE_H
#define RBTREE_H

#include "Observer.h"

#include <functional>
#include <unistd.h>

namespace rbtree {
enum class Color;
struct Node;
class RBTree;
} // namespace rbtree

enum class Color { RED, BLACK };

struct Node
{
    int key;
    Node *left;
    Node *right;
    Node *parent;
    Color color;

    explicit Node(int key);

    Node(int key, Node *left, Node *right, Node *parent, Color color);

    Node *sibling();
};

class RBTree
{
public:
    struct DrawData
    {
        Node *&root;
        enum class Status { DEFAULT, FOUND, PASSING, DELETED };
        std::pair<Node *, Status> changedNode;

        DrawData &operator=(const RBTree::DrawData &dt);
    };

    RBTree();

    RBTree(std::initializer_list<int> list);

    ~RBTree();

    void insert(int key);

    [[nodiscard]] int size() const;

    [[nodiscard]] bool empty() const;

    int *lowerBound(int key);

    int *upperBound(int key);

    int *find(int key);

    void erase(const int &key);

    void clear();

    void inOrder(Node *node);

    void subscribe(Observer<DrawData> *observer);

    Node *root{};

private:
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

    Observable<DrawData> drawData_ = DrawData{root};

    int size_;
};

#endif // RBTREE_H
