#include "rbtree.h"

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

RBTree::DrawData &RBTree::DrawData::operator=(const DrawData &dt)
{
    if (this == &dt) {
        return *this;
    }

    this->root = dt.root;
    this->changedNode = dt.changedNode;

    return *this;
}

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

void RBTree::insert(int key)
{
    if (root == nullptr) {
        ++size_;
        root = new Node(key, nullptr, nullptr, nullptr, Color::BLACK);
        drawData_.set({root});
        return;
    }

    Node *t_node = root;
    Node *p_node;
    while (t_node) {
        drawData_.set({root, {t_node, DrawData::Status::PASSING}});
        p_node = t_node;
        if (key < t_node->key) {
            t_node = t_node->left;
        } else if (t_node->key < key) {
            t_node = t_node->right;
        } else {
            drawData_.set({root});
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

    drawData_.set({root, {inserted_node, DrawData::Status::FOUND}});

    if (inserted_node->parent->parent == nullptr) {
        drawData_.set({root});
        return;
    }

    fixTreeInsert(inserted_node);

    drawData_.set({root});
}

int RBTree::size() const
{
    return size_;
}

bool RBTree::empty() const
{
    return root == nullptr;
}

int *RBTree::lowerBound(int key)
{
    Node *x = root;
    Node *min = nullptr;
    while (x != nullptr) {
        drawData_.set({root, {x, RBTree::DrawData::Status::PASSING}});
        if (x->key == key) {
            drawData_.set({root, {x, RBTree::DrawData::Status::FOUND}});
            return &x->key;
        } else if (x->key > key) {
            min = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    if (min == nullptr) {
        drawData_.set({root});
        return nullptr;
    }

    drawData_.set({root, {min, RBTree::DrawData::Status::FOUND}});
    return &min->key;
}

int *RBTree::upperBound(int key)
{
    Node *x = root;
    Node *min = nullptr;
    while (x != nullptr) {
        drawData_.set({root, {x, RBTree::DrawData::Status::PASSING}});
        if (x->key > key) {
            min = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    if (min == nullptr) {
        drawData_.set({root});
        return nullptr;
    }

    drawData_.set({root, {min, RBTree::DrawData::Status::FOUND}});
    return &min->key;
}

int *RBTree::find(int key)
{
    Node *t_node = root;
    while (t_node) {
        drawData_.set({root, {t_node, RBTree::DrawData::Status::PASSING}});
        if (key < t_node->key) {
            t_node = t_node->left;
        } else if (t_node->key < key) {
            t_node = t_node->right;
        } else {
            drawData_.set({root, {t_node, RBTree::DrawData::Status::FOUND}});
            return &t_node->key;
        }
    }

    drawData_.set({root});
    return nullptr;
}

void RBTree::erase(const int &key)
{
    Node *a = findNode(key);
    if (a != nullptr) {
        eraseNode(a);
    }

    drawData_.set({root});
}

void RBTree::clear()
{
    nodeDestructor(root);
    size_ = 0;
    root = nullptr;
    drawData_.set({root});
}

void RBTree::inOrder(Node *node)
{
    if (node == nullptr) {
        return;
    }

    inOrder(node->left);
    drawData_.set({root, {node, DrawData::Status::PASSING}});
    inOrder(node->right);

    if (node == root) {
        drawData_.set({root});
    }
}

void RBTree::subscribe(Observer<DrawData> *observer)
{
    drawData_.subscribe(observer);
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

void RBTree::fixTreeInsert(Node *node)
{
    if (node->parent == nullptr) {
        node->color = Color::BLACK;
        drawData_.set({root});
        return;
    }

    if (node->parent->color == Color::BLACK) {
        drawData_.set({root, {node->parent, DrawData::Status::PASSING}});
        return;
    }

    Node *uncle = findUncle(node);
    Node *grandpa = findGrandparent(node);

    if (uncle && uncle->color == Color::RED) {
        drawData_.set({root, {uncle, DrawData::Status::FOUND}});

        node->parent->color = Color::BLACK;
        drawData_.set({root});

        uncle->color = Color::BLACK;
        drawData_.set({root});

        grandpa->color = Color::RED;
        drawData_.set({root});

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
    drawData_.set({root});

    grandpa->color = Color::RED;
    drawData_.set({root});

    if (node->key < node->parent->key) {
        rightRotation(grandpa);
    } else {
        leftRotation(grandpa);
    }
}

Node *RBTree::findUncle(Node *node)
{
    Node *grandpa = findGrandparent(node);
    if (grandpa == nullptr) {
        return nullptr;
    }
    if (node->key < grandpa->key) {
        drawData_.set({root, {grandpa->right, DrawData::Status::PASSING}});
        return grandpa->right;
    }
    drawData_.set({root, {grandpa->left, DrawData::Status::PASSING}});
    return grandpa->left;
}

Node *RBTree::findGrandparent(Node *node)
{
    if (node->parent == nullptr || node->parent->parent == nullptr) {
        return nullptr;
    }
    drawData_.set({root, {node->parent->parent, DrawData::Status::FOUND}});
    return node->parent->parent;
}

void RBTree::leftRotation(Node *node)
{
    Node *p = node->parent;
    drawData_.set({root, {p, DrawData::Status::PASSING}});

    Node *r = node->right;
    drawData_.set({root, {r, DrawData::Status::PASSING}});

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

    drawData_.set({root});
}

void RBTree::rightRotation(Node *node)
{
    Node *p = node->parent;
    drawData_.set({root, {p, DrawData::Status::PASSING}});

    Node *l = node->left;
    drawData_.set({root, {l, DrawData::Status::PASSING}});

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

    drawData_.set({root});
}

void RBTree::eraseNode(Node *node)
{
    drawData_.set({root, {node, DrawData::Status::FOUND}});

    Node *replacement_node = findReplacementNode(node);
    drawData_.set({root, {replacement_node, DrawData::Status::FOUND}});

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
                Node *sibling = node->sibling();
                if (sibling) {
                    drawData_.set({root, {sibling, DrawData::Status::PASSING}});
                    sibling->color = Color::RED;
                    drawData_.set({root});
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

Node *RBTree::findReplacementNode(Node *node)
{
    if (node->left && node->right) {
        Node *replacement = node->right;
        drawData_.set({root, {replacement, DrawData::Status::PASSING}});
        while (replacement->left) {
            drawData_.set({root, {replacement->left, DrawData::Status::PASSING}});
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

void RBTree::repairDoubleBlack(Node *node)
{
    drawData_.set({root, {node, DrawData::Status::FOUND}});

    if (node == root) {
        return;
    }

    Node *sibling = node->sibling();
    Node *parent = node->parent;

    if (sibling == nullptr) {
        repairDoubleBlack(parent);
    } else {
        if (sibling->color == Color::RED) {
            drawData_.set({root, {sibling, DrawData::Status::FOUND}});
            sibling->color = Color::BLACK;

            drawData_.set({root, {parent, DrawData::Status::PASSING}});
            parent->color = Color::RED;
            drawData_.set({root});

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
                    drawData_.set({root, {sibling->left, DrawData::Status::PASSING}});
                    if (sibling == sibling->parent->left) {
                        sibling->left->color = sibling->color;
                        drawData_.set({root});

                        drawData_.set({root, {sibling, DrawData::Status::PASSING}});
                        sibling->color = parent->color;
                        drawData_.set({root});

                        rightRotation(parent);
                    } else {
                        sibling->left->color = parent->color;
                        drawData_.set({root});

                        rightRotation(sibling);
                        leftRotation(parent);
                    }
                } else {
                    drawData_.set({root, {sibling->right, DrawData::Status::PASSING}});
                    if (sibling == sibling->parent->left) {
                        sibling->right->color = parent->color;
                        drawData_.set({root});

                        leftRotation(sibling);
                        rightRotation(parent);
                    } else {
                        sibling->right->color = sibling->color;
                        drawData_.set({root});

                        drawData_.set({root, {sibling, DrawData::Status::PASSING}});
                        sibling->color = parent->color;
                        drawData_.set({root});

                        leftRotation(parent);
                    }
                }
                drawData_.set({root, {parent, DrawData::Status::PASSING}});
                parent->color = Color::BLACK;
                drawData_.set({root});
            } else {
                drawData_.set({root, {sibling, DrawData::Status::PASSING}});
                sibling->color = Color::RED;
                drawData_.set({root});

                if (parent->color == Color::BLACK) {
                    repairDoubleBlack(parent);
                } else {
                    drawData_.set({root, {parent, DrawData::Status::PASSING}});
                    parent->color = Color::BLACK;
                    drawData_.set({root});
                }
            }
        }
    }
}

Node *RBTree::findNode(int key)
{
    Node *t_node = root;
    while (t_node) {
        drawData_.set({root, {t_node, DrawData::Status::PASSING}});
        if (key < t_node->key) {
            t_node = t_node->left;
        } else if (t_node->key < key) {
            t_node = t_node->right;
        } else {
            drawData_.set({root, {t_node, DrawData::Status::FOUND}});
            return t_node;
        }
    }
    return nullptr;
}
