#include "sealib/collection/avltree.h"

namespace Sealib {

uint64_t AVLTree::search(uint64_t k) const {
    uint64_t r = INVALID;
    if (root != nullptr) {
        Cell *u = root;
        while (u != nullptr) {
            u = k < u->key ? u->left
                           : k > u->key ? u->right : (r = u->data, nullptr);
        }
    }
    return r;
}

void AVLTree::insert(uint64_t k, uint64_t v) {
    if (root != nullptr) {
        Cell *u = root;
        bool done = false;
        while (!done) {
            if (k < u->key) {
                if (u->left == nullptr) {
                    u->left = new Cell(k, v, u);
                    rebalanceParents(u->left, AVL_LEFT);
                    done = true;
                } else {
                    u = u->left;
                }
            } else if (k > u->key) {
                if (u->right == nullptr) {
                    u->right = new Cell(k, v, u);
                    rebalanceParents(u->right, AVL_RIGHT);
                    done = true;
                } else {
                    u = u->right;
                }
            } else {
                u->data = v;
                done = true;
            }
        }
    } else {
        root = new Cell(k, v);
    }
}  // namespace Sealib

void AVLTree::rebalanceParents(Cell *x, uint8_t xSide) {
    Cell *head = x->parent;
    uint8_t side = xSide;
    // correct 0 0 cells' balances
    while (head->bal == AVL_BALANCED && head->parent != nullptr) {
        head->bal = side;
        side = head->key > head->parent->key ? AVL_RIGHT : AVL_LEFT;
        head = head->parent;
        // loop ends when chain head is reached
    }
    if (head->parent == nullptr && head->bal == AVL_BALANCED) {
        head->bal = side;
    } else if (head->bal == !side) {
        // inserted at short branch => already balanced
        head->bal = AVL_BALANCED;
    } else {
        // inserted at long branch => rebalance
        Cell *chainTop;
        if (head->right == nullptr ||
            (head->left != nullptr && head->left->bal != AVL_BALANCED)) {
            chainTop = head->left;
        } else {
            chainTop = head->right;
        }
        if (x->parent == chainTop && chainTop->bal == !head->bal) {
            swapLeaves(chainTop);
        } else if (chainTop->bal == head->bal) {
            rotateTree(chainTop);
        } else if (chainTop->bal == !head->bal) {
            spliceTree(chainTop);
        }
    }
}

void AVLTree::swapLeaves(Cell *a) {
    Cell *x = a->bal == AVL_LEFT ? a->left : a->right;
    Cell *s = a->parent;
    x->parent = s->parent;
    if (x->parent == nullptr)
        root = x;
    else if (x->parent->left == s)
        x->parent->left = x;
    else
        x->parent->right = x;
    s->parent = a->parent = x;
    s->left = s->right = nullptr;
    a->left = a->right = nullptr;
    x->left = a->bal == AVL_LEFT ? s : a;
    x->right = a->bal == AVL_LEFT ? a : s;
    x->bal = s->bal = a->bal = AVL_BALANCED;
}

void AVLTree::rotateTree(Cell *a) {
    Cell *s = a->parent;
    Cell *tx, *t1, *t2;
    if (a->bal == AVL_LEFT) {
        t1 = s->right;
        t2 = a->right;
        tx = a->left;
    } else {
        t1 = s->left;
        t2 = a->left;
        tx = a->right;
    }
    a->parent = s->parent;
    if (a->parent == nullptr)
        root = a;
    else if (a->parent->left == s)
        a->parent->left = a;
    else
        a->parent->right = a;

    s->parent = a;
    if (t2 != nullptr) t2->parent = s;
    if (a->bal == AVL_LEFT) {
        s->left = t2;
        a->right = s;
    } else {
        a->left = s;
        s->right = t2;
    }
    s->bal = a->bal = AVL_BALANCED;
}

void AVLTree::spliceTree(Cell *a) {
    Cell *s = a->parent;
    Cell *b, *t1, *t2, *t3, *tx;
    if (a->bal == AVL_RIGHT) {
        b = a->right;
        t1 = s->right;
        t2 = a->left;
    } else {
        b = a->left;
        t1 = s->left;
        t2 = a->right;
    }
    if (b->bal == AVL_LEFT) {
        tx = b->left;
        t3 = b->right;
    } else {
        tx = b->right;
        t3 = b->left;
    }
    b->parent = s->parent;
    if (b->parent == nullptr)
        root = b;
    else if (b->parent->left == s)
        b->parent->left = b;
    else
        b->parent->right = b;
    a->parent = s->parent = b;
    tx->parent = a;
    if (t3 != nullptr) t3->parent = s;
    if (a->bal == AVL_RIGHT) {
        s->left = t3;
        a->right = tx;
        b->left = a;
        b->right = s;
    } else {
        s->right = t3;
        a->left = tx;
        b->left = s;
        b->right = a;
    }
    if (b->bal == AVL_LEFT) {
        s->bal = AVL_RIGHT;
        a->bal = AVL_BALANCED;
    } else {
        s->bal = AVL_BALANCED;
        a->bal = AVL_LEFT;
    }
    b->bal = AVL_BALANCED;
}

}  // namespace Sealib