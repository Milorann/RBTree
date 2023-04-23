#include "rbtree.h"

template <typename T>
Node<T>::Node(T key) {
        this->key = key;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        color = Color::RED;
}

template <typename T>
    Node<T>::Node(T key, Node *left, Node *right, Node *parent, Color color)
        : key(key), left(left), right(right), parent(parent), color(color) {
    }

template <typename T>
void RBTree<T>::insert(T key) {
    if (root == nullptr) {
        ++size_;
        root = new Node<T>(key, nullptr, nullptr, nullptr, Color::BLACK);
        return;
    }

    Node<T> *t_node = root;
    Node<T> *p_node;
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

    auto *inserted_node = new Node<T>(key);
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

template <typename T>
void RBTree<T>::fixTreeInsert(Node<T> *node) {
    if (node->parent == nullptr) {
        node->color = Color::BLACK;
        return;
    }

    if (node->parent->color == Color::BLACK) {
        return;
    }

    Node<T> *uncle = findUncle(node);
    Node<T> *grandpa = findGrandparent(node);

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

template <typename T>
void RBTree<T>::leftRotation(Node<T> *node) {
    Node<T> *p = node->parent;
    Node<T> *r = node->right;
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

template <typename T>
void RBTree<T>::rightRotation(Node<T> *node) {
    Node<T> *p = node->parent;
    Node<T> *l = node->left;
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

template <typename T>
Node<T> *RBTree<T>::findUncle(Node<T> *node) {
    Node<T> *grandpa = findGrandparent(node);
    if (grandpa == nullptr) {
        return nullptr;
    }
    if (node->key < grandpa->key) {
        return grandpa->right;
    }
    return grandpa->left;
}

template <typename T>
Node<T> *RBTree<T>::findGrandparent(Node<T> *node) {
    if (node->parent == nullptr || node->parent->parent == nullptr) {
        return nullptr;
    }
    return node->parent->parent;
}

template <typename T>
void RBTree<T>::nodeDestructor(Node<T> *node) {
    if (node == nullptr) {
        return;
    }
    nodeDestructor(node->left);
    nodeDestructor(node->right);
    delete node;
}

template <typename T>
T *RBTree<T>::find(T key) const {
    Node<T> *t_node = root;
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

template <typename T>
T *RBTree<T>::lowerBound(T key) const {
    Node<T> *x = root;
    Node<T> *min = nullptr;
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

template <typename T>
bool RBTree<T>::empty() const {
    return root == nullptr;
}

template <typename T>
int RBTree<T>::size() const {
    return size_;
}

template <typename T>
RBTree<T>::~RBTree() {
    nodeDestructor(root);
}

template <typename T>
RBTree<T>::RBTree(std::initializer_list<T> list) {
    size_ = 0;
    for (auto el : list) {
        insert(el);
    }
}

template <typename T>
RBTree<T>::RBTree() {
    root = nullptr;
    size_ = 0;
}


