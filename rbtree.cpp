#include "rbtree.h"

RBTree::RBTree()
{
    root = nullptr;
    size_ = 0;
}

RBTree::RBTree(std::initializer_list<int> list)
{
    size_ = 0;
    for (auto el : list) {
        insert(el);
    }
}

RBTree::~RBTree()
{
    nodeDestructor(root);
}

int *RBTree::upperBound(int key) const
{
    Node *x = root;
    Node *min = nullptr;
    while (x != nullptr) {
        if (x->key > key) {
            min = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    if (min == nullptr) {
        return nullptr;
    }
    return &min->key;
}

Node *RBTree::findNode(int key)
{
    Node *t_node = root;
    while (t_node) {
        if (key < t_node->key) {
            t_node = t_node->left;
        } else if (t_node->key < key) {
            t_node = t_node->right;
        } else {
            return t_node;
        }
    }
    return nullptr;
}

void RBTree::repairDoubleBlack(Node *node)
{
    if (node == root) {
        return;
    }

    Node *sibling = node->sibling();
    Node *parent = node->parent;

    if (sibling == nullptr) {
        repairDoubleBlack(parent);
    } else {
        if (sibling->color == Color::RED) {
            parent->color = Color::RED;
            sibling->color = Color::BLACK;
            if (sibling == sibling->parent->left) {
                rightRotation(parent);
            } else {
                leftRotation(parent);
            }
            repairDoubleBlack(node);
        } else {
            if ((sibling->left && sibling->left->color == Color::RED)
                || (sibling->right && sibling->right->color == Color::RED)) {
                if (sibling->left && sibling->left->color == Color::RED) {
                    if (sibling == sibling->parent->left) {
                        sibling->left->color = sibling->color;
                        sibling->color = parent->color;
                        rightRotation(parent);
                    } else {
                        sibling->left->color = parent->color;
                        rightRotation(sibling);
                        leftRotation(parent);
                    }
                } else {
                    if (sibling == sibling->parent->left) {
                        sibling->right->color = parent->color;
                        leftRotation(sibling);
                        rightRotation(parent);
                    } else {
                        sibling->right->color = sibling->color;
                        sibling->color = parent->color;
                        leftRotation(parent);
                    }
                }
                parent->color = Color::BLACK;
            } else {
                sibling->color = Color::RED;
                if (parent->color == Color::BLACK) {
                    repairDoubleBlack(parent);
                } else {
                    parent->color = Color::BLACK;
                }
            }
        }
    }
}

Node *Node::sibling()
{
    if (parent == nullptr) {
        return nullptr;
    }
    if (this == parent->left) {
        return parent->right;
    }
    return parent->left;
}

Node *RBTree::findReplacementNode(Node *node)
{
    if (node->left && node->right) {
        Node *replacement = node->right;
        while (replacement->left) {
            replacement = replacement->left;
        }
        return replacement;
    }
    if (node->left == nullptr && node->right == nullptr) {
        return nullptr;
    }
    if (node->left) {
        return node->left;
    } else {
        return node->right;
    }
}

void RBTree::eraseNode(Node *node)
{
    Node *replacement_node = findReplacementNode(node);

    bool both_black = false;
    if ((replacement_node == nullptr || replacement_node->color == Color::BLACK)
        && (node->color == Color::BLACK)) {
        both_black = true;
    }

    Node *parent = node->parent;

    if (replacement_node == nullptr) {
        if (node == root) {
            root = nullptr;
        } else {
            if (!both_black) {
                if (node->sibling()) {
                    node->sibling()->color = Color::RED;
                }
            } else {
                repairDoubleBlack(node);
            }
            if (node == node->parent->left) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
        }
        delete node;
        return;
    }

    if (node->left == nullptr || node->right == nullptr) {
        if (node == root) {
            node->key = replacement_node->key;
            node->left = nullptr;
            node->right = nullptr;
            delete replacement_node;
        } else {
            if (node != node->parent->left) {
                parent->right = replacement_node;
            } else {
                parent->left = replacement_node;
            }
            delete node;
            replacement_node->parent = parent;
            if (!both_black) {
                replacement_node->color = Color::BLACK;
            } else {
                repairDoubleBlack(replacement_node);
            }
        }
        return;
    }

    int swap_key = replacement_node->key;
    replacement_node->key = node->key;
    node->key = swap_key;
    eraseNode(replacement_node);
}

void RBTree::erase(const int &key)
{
    Node *a = findNode(key);
    if (a != nullptr) {
        eraseNode(a);
    }
}

void RBTree::subscribe(Observer<DrawData> *observer)
{
    drawData_.subscribe(observer);
}

Node::Node(int key)
{
    this->key = key;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    color = Color::RED;
}

Node::Node(int key, Node *left, Node *right, Node *parent, Color color)
    : key(key)
    , left(left)
    , right(right)
    , parent(parent)
    , color(color)
{}

void RBTree::insert(int key)
{
    if (root == nullptr) {
        ++size_;
        root = new Node(key, nullptr, nullptr, nullptr, Color::BLACK);
        return;
    }

    Node *t_node = root;
    Node *p_node;
    while (t_node) {
        p_node = t_node;
        if (key < t_node->key) {
            t_node = t_node->left;
        } else if (t_node->key < key) {
            t_node = t_node->right;
        } else {
            return;
        }
    }

    auto *inserted_node = new Node(key);
    ++size_;

    inserted_node->parent = p_node;
    if (inserted_node->key < p_node->key) {
        p_node->left = inserted_node;
    } else {
        p_node->right = inserted_node;
    }

    if (inserted_node->parent->parent == nullptr) {
        return;
    }

    fixTreeInsert(inserted_node);
}

void RBTree::fixTreeInsert(Node *node)
{
    if (node->parent == nullptr) {
        node->color = Color::BLACK;
        return;
    }

    if (node->parent->color == Color::BLACK) {
        return;
    }

    Node *uncle = findUncle(node);
    Node *grandpa = findGrandparent(node);

    if (uncle && uncle->color == Color::RED) {
        node->parent->color = Color::BLACK;
        uncle->color = Color::BLACK;
        grandpa->color = Color::RED;
        fixTreeInsert(grandpa);
        return;
    }

    if ((node->parent->key < node->key) && (node->parent->key < grandpa->key)) {
        leftRotation(node->parent);
        node = node->left;
        grandpa = findGrandparent(node);
    } else if ((node->key < node->parent->key) && (grandpa->key < node->parent->key)) {
        rightRotation(node->parent);
        node = node->right;
        grandpa = findGrandparent(node);
    }

    node->parent->color = Color::BLACK;
    grandpa->color = Color::RED;
    if (node->key < node->parent->key) {
        rightRotation(grandpa);
    } else {
        leftRotation(grandpa);
    }
}

void RBTree::leftRotation(Node *node)
{
    Node *p = node->parent;
    Node *r = node->right;
    node->right = r->left;
    if (r->left) {
        r->left->parent = node;
    }
    r->left = node;
    node->parent = r;

    r->parent = p;
    if (p == nullptr) {
        root = r;
        return;
    }
    if (r->key < p->key) {
        p->left = r;
    } else {
        p->right = r;
    }
}

void RBTree::rightRotation(Node *node)
{
    Node *p = node->parent;
    Node *l = node->left;
    node->left = l->right;
    if (l->right) {
        l->right->parent = node;
    }
    l->right = node;
    node->parent = l;

    l->parent = p;
    if (p == nullptr) {
        root = l;
        return;
    }
    if (l->key < p->key) {
        p->left = l;
    } else {
        p->right = l;
    }
}

Node *RBTree::findUncle(Node *node)
{
    Node *grandpa = findGrandparent(node);
    if (grandpa == nullptr) {
        return nullptr;
    }
    if (node->key < grandpa->key) {
        return grandpa->right;
    }
    return grandpa->left;
}

Node *RBTree::findGrandparent(Node *node)
{
    if (node->parent == nullptr || node->parent->parent == nullptr) {
        return nullptr;
    }
    return node->parent->parent;
}

void RBTree::nodeDestructor(Node *node)
{
    if (node == nullptr) {
        return;
    }
    nodeDestructor(node->left);
    nodeDestructor(node->right);
    delete node;
}

int *RBTree::find(int key) const
{
    Node *t_node = root;
    while (t_node) {
        if (key < t_node->key) {
            t_node = t_node->left;
        } else if (t_node->key < key) {
            t_node = t_node->right;
        } else {
            return &t_node->key;
        }
    }
    return nullptr;
}

int *RBTree::lowerBound(int key) const
{
    Node *x = root;
    Node *min = nullptr;
    while (x != nullptr) {
        if (x->key == key) {
            return &x->key;
        } else if (x->key > key) {
            min = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    if (min == nullptr) {
        return nullptr;
    }
    return &min->key;
}

bool RBTree::empty() const
{
    return root == nullptr;
}

int RBTree::size() const
{
    return size_;
}
