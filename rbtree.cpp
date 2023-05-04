#include "rbtree.h"

namespace rbtree {
RBTree::RBTree(std::initializer_list<int> list)
{
    size_ = 0;
    for (auto el : list) {
        insert(el);
    }
}

RBTree::~RBTree()
{
    destroySubtree(root_);
}

void RBTree::insert(int key)
{
    if (root_ == nullptr) {
        ++size_;
        root_ = new Node{key, Color::Black};
        drawPort_.set({root_});
        return;
    }

    Node *p_node = findInsertionParent(key);
    if (p_node->key == key) {
        return;
    }

    auto *inserted_node = new Node{key};
    ++size_;

    inserted_node->parent = p_node;
    if (inserted_node->key < p_node->key) {
        p_node->left = inserted_node;
    } else {
        p_node->right = inserted_node;
    }
    drawPort_.set({root_, {inserted_node, DrawData::Status::Found}});

    rebalanceAfterInsert(inserted_node);
    drawPort_.set({root_});
}

int RBTree::size() const
{
    return size_;
}

bool RBTree::empty() const
{
    return root_ == nullptr;
}

int RBTree::lowerBound(int key)
{
    Node *x = root_;
    Node *min = nullptr;
    while (x != nullptr) {
        drawPort_.set({root_, {x, RBTree::DrawData::Status::Passing}});
        if (x->key == key) {
            drawPort_.set({root_, {x, RBTree::DrawData::Status::Found}});
            return x->key;
        } else if (x->key > key) {
            min = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    if (min == nullptr) {
        drawPort_.set({root_});
        return -1;
    }

    drawPort_.set({root_, {min, RBTree::DrawData::Status::Found}});
    return min->key;
}

int RBTree::upperBound(int key)
{
    Node *x = root_;
    Node *min = nullptr;
    while (x != nullptr) {
        drawPort_.set({root_, {x, RBTree::DrawData::Status::Passing}});
        if (x->key > key) {
            min = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    if (min == nullptr) {
        drawPort_.set({root_});
        return -1;
    }

    drawPort_.set({root_, {min, RBTree::DrawData::Status::Found}});
    return min->key;
}

int RBTree::find(int key)
{
    Node *node = findNode(key);
    if (node == nullptr) {
        drawPort_.set({root_});
        return -1;
    }

    return node->key;
}

void RBTree::erase(int key)
{
    Node *node = findNode(key);
    if (node != nullptr) {
        eraseNode(node);
    }

    drawPort_.set({root_});
}

void RBTree::clear()
{
    destroySubtree(root_);
    size_ = 0;
    root_ = nullptr;
    drawPort_.set({root_});
}

void RBTree::subscribe(Observer<DrawData> *observer)
{
    drawPort_.subscribe(observer);
}

const Node *RBTree::root() const
{
    return root_;
}

void RBTree::destroySubtree(Node *node)
{
    if (node == nullptr) {
        return;
    }
    destroySubtree(node->left);
    destroySubtree(node->right);
    delete node;
}

void RBTree::rebalanceAfterInsert(Node *node)
{
    if (node->parent == nullptr) {
        node->color = Color::Black;
        drawPort_.set({root_});
        return;
    }

    if (node->parent->color == Color::Black) {
        drawPort_.set({root_, {node->parent, DrawData::Status::Passing}});
        return;
    }

    Node *uncle = findUncle(node);
    Node *grandpa = findGrandparent(node);

    if (uncle && uncle->color == Color::Red) {
        drawPort_.set({root_, {uncle, DrawData::Status::Found}});

        node->parent->color = Color::Black;
        drawPort_.set({root_});

        uncle->color = Color::Black;
        drawPort_.set({root_});

        grandpa->color = Color::Red;
        drawPort_.set({root_});

        rebalanceAfterInsert(grandpa);
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

    node->parent->color = Color::Black;
    drawPort_.set({root_});

    grandpa->color = Color::Red;
    drawPort_.set({root_});

    if (node->key < node->parent->key) {
        rightRotation(grandpa);
    } else {
        leftRotation(grandpa);
    }
}

Node *RBTree::findSibling(Node *node)
{
    if (node->parent == nullptr) {
        return nullptr;
    }
    if (node == node->parent->left) {
        return node->parent->right;
    }
    return node->parent->left;
}

Node *RBTree::findUncle(Node *node)
{
    Node *grandpa = findGrandparent(node);
    if (grandpa == nullptr) {
        return nullptr;
    }
    if (node->key < grandpa->key) {
        drawPort_.set({root_, {grandpa->right, DrawData::Status::Passing}});
        return grandpa->right;
    }
    drawPort_.set({root_, {grandpa->left, DrawData::Status::Passing}});
    return grandpa->left;
}

Node *RBTree::findGrandparent(Node *node)
{
    if (node->parent == nullptr || node->parent->parent == nullptr) {
        return nullptr;
    }
    drawPort_.set({root_, {node->parent->parent, DrawData::Status::Found}});
    return node->parent->parent;
}

Node *RBTree::findNode(int key)
{
    Node *t_node = root_;
    while (t_node) {
        drawPort_.set({root_, {t_node, DrawData::Status::Passing}});
        if (key < t_node->key) {
            t_node = t_node->left;
        } else if (t_node->key < key) {
            t_node = t_node->right;
        } else {
            drawPort_.set({root_, {t_node, DrawData::Status::Found}});
            return t_node;
        }
    }
    return nullptr;
}

Node *RBTree::findInsertionParent(int key)
{
    Node *t_node = root_;
    Node *p_node;
    while (t_node) {
        drawPort_.set({root_, {t_node, DrawData::Status::Passing}});
        p_node = t_node;
        if (key < t_node->key) {
            t_node = t_node->left;
        } else if (t_node->key < key) {
            t_node = t_node->right;
        } else {
            drawPort_.set({root_});
            return p_node;
        }
    }
    return p_node;
}

Node *RBTree::findReplacementNode(Node *node)
{
    if (node->left && node->right) {
        Node *replacement = node->right;
        drawPort_.set({root_, {replacement, DrawData::Status::Passing}});
        while (replacement->left) {
            drawPort_.set({root_, {replacement->left, DrawData::Status::Passing}});
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

void RBTree::leftRotation(Node *node)
{
    Node *p = node->parent;
    drawPort_.set({root_, {p, DrawData::Status::Passing}});

    Node *r = node->right;
    drawPort_.set({root_, {r, DrawData::Status::Passing}});

    node->right = r->left;
    if (r->left) {
        r->left->parent = node;
    }
    r->left = node;
    node->parent = r;

    r->parent = p;
    if (p == nullptr) {
        root_ = r;
        return;
    }
    if (r->key < p->key) {
        p->left = r;
    } else {
        p->right = r;
    }

    drawPort_.set({root_});
}

void RBTree::rightRotation(Node *node)
{
    Node *p = node->parent;
    drawPort_.set({root_, {p, DrawData::Status::Passing}});

    Node *l = node->left;
    drawPort_.set({root_, {l, DrawData::Status::Passing}});

    node->left = l->right;
    if (l->right) {
        l->right->parent = node;
    }
    l->right = node;
    node->parent = l;

    l->parent = p;
    if (p == nullptr) {
        root_ = l;
        return;
    }
    if (l->key < p->key) {
        p->left = l;
    } else {
        p->right = l;
    }

    drawPort_.set({root_});
}

void RBTree::eraseNode(Node *node)
{
    drawPort_.set({root_, {node, DrawData::Status::Found}});
    Node *replacement_node = findReplacementNode(node);
    drawPort_.set({root_, {replacement_node, DrawData::Status::Found}});

    bool both_black = false;
    if ((replacement_node == nullptr || replacement_node->color == Color::Black)
        && (node->color == Color::Black)) {
        both_black = true;
    }

    if (replacement_node == nullptr && node == root_) {
        root_ = nullptr;
        delete node;
        return;
    }

    Node *parent = node->parent;
    if (replacement_node == nullptr) {
        Node *sibling = findSibling(node);
        if (!both_black && sibling) {
            drawPort_.set({root_, {sibling, DrawData::Status::Passing}});
            sibling->color = Color::Red;
            drawPort_.set({root_});
        } else if (both_black) {
            rebalanceAfterDeletion(node);
        }
        if (node == node->parent->left) {
            parent->left = nullptr;
        } else {
            parent->right = nullptr;
        }
        delete node;
        return;
    }
    if ((node->left == nullptr || node->right == nullptr) && node == root_) {
        node->key = replacement_node->key;
        node->left = nullptr;
        node->right = nullptr;
        delete replacement_node;
        return;
    }
    if (node->left == nullptr || node->right == nullptr) {
        if (node != node->parent->left) {
            parent->right = replacement_node;
        } else {
            parent->left = replacement_node;
        }
        delete node;

        replacement_node->parent = parent;
        if (!both_black) {
            replacement_node->color = Color::Black;
        } else {
            rebalanceAfterDeletion(replacement_node);
        }
        return;
    }

    int swap_key = replacement_node->key;
    replacement_node->key = node->key;
    node->key = swap_key;
    eraseNode(replacement_node);
}

void RBTree::rebalanceAfterDeletion(Node *node)
{
    drawPort_.set({root_, {node, DrawData::Status::Found}});
    if (node == root_) {
        return;
    }
    Node *sibling = findSibling(node);
    Node *parent = node->parent;
    if (sibling == nullptr) {
        rebalanceAfterDeletion(parent);
        return;
    }

    if (sibling->color == Color::Red) {
        drawPort_.set({root_, {sibling, DrawData::Status::Found}});
        sibling->color = Color::Black;
        drawPort_.set({root_, {parent, DrawData::Status::Passing}});
        parent->color = Color::Red;
        drawPort_.set({root_});

        if (sibling == sibling->parent->left) {
            rightRotation(parent);
        } else {
            leftRotation(parent);
        }
        rebalanceAfterDeletion(node);
        return;
    }
    if (sibling->left && sibling->left->color == Color::Red) {
        drawPort_.set({root_, {sibling->left, DrawData::Status::Passing}});
        if (sibling == sibling->parent->left) {
            sibling->left->color = sibling->color;
            drawPort_.set({root_});

            drawPort_.set({root_, {sibling, DrawData::Status::Passing}});
            sibling->color = parent->color;
            drawPort_.set({root_});

            rightRotation(parent);
        } else {
            sibling->left->color = parent->color;
            drawPort_.set({root_});

            rightRotation(sibling);
            leftRotation(parent);
        }
        drawPort_.set({root_, {parent, DrawData::Status::Passing}});
        parent->color = Color::Black;
        drawPort_.set({root_});
        return;
    }
    if (sibling->right && sibling->right->color == Color::Red) {
        drawPort_.set({root_, {sibling->right, DrawData::Status::Passing}});
        if (sibling == sibling->parent->left) {
            sibling->right->color = parent->color;
            drawPort_.set({root_});

            leftRotation(sibling);
            rightRotation(parent);
        } else {
            sibling->right->color = sibling->color;
            drawPort_.set({root_});

            drawPort_.set({root_, {sibling, DrawData::Status::Passing}});
            sibling->color = parent->color;
            drawPort_.set({root_});

            leftRotation(parent);
        }
        drawPort_.set({root_, {parent, DrawData::Status::Passing}});
        parent->color = Color::Black;
        drawPort_.set({root_});
        return;
    }

    drawPort_.set({root_, {sibling, DrawData::Status::Passing}});
    sibling->color = Color::Red;
    drawPort_.set({root_});
    if (parent->color == Color::Black) {
        rebalanceAfterDeletion(parent);
        return;
    }

    drawPort_.set({root_, {parent, DrawData::Status::Passing}});
    parent->color = Color::Black;
    drawPort_.set({root_});
}
} // namespace rbtree
