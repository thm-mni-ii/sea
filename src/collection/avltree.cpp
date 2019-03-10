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
                    rebalanceChain(u, AVL_LEFT);
                    done = true;
                } else {
                    u = u->left;
                }
            } else if (k > u->key) {
                if (u->right == nullptr) {
                    u->right = new Cell(k, v, u);
                    rebalanceChain(u, AVL_RIGHT);
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
}

void AVLTree::remove(uint64_t k) {
    if (root != nullptr) {
        Cell *u = root;
        bool done = false;
        while (!done) {
            if (k < u->key) {
                if (u->left != nullptr) {
                    u = u->left;
                } else {
                    done = true;
                }
            } else if (k > u->key) {
                if (u->right != nullptr) {
                    u = u->right;
                } else {
                    done = true;
                }
            } else {
                Cell *p = u->parent;
                // ins = where to hang new child
                Cell **ins = u->parent == nullptr
                                 ? &root
                                 : p->left == u ? &((*p).left) : &((*p).right);
                if (u->left == nullptr && u->right == nullptr) {
                    if (p == nullptr) {
                        root = nullptr;
                    } else {
                        *ins = nullptr;
                        if (p->parent != nullptr) {
                            uint8_t side = p->left == u ? AVL_RIGHT : AVL_LEFT;
                            p->parent->bal = !side;
                            rebalanceChain(p, side);
                        }
                    }
                } else if (u->left == nullptr || u->right == nullptr) {
                    Cell *v = u->bal == AVL_LEFT ? u->left : u->right;
                    *ins = v;
                    v->parent = p;
                    if (p != nullptr) rebalanceChain(p, u->bal);
                } else {
                    Cell *m = u->right;
                    while (m->left != nullptr) {
                        m->bal = AVL_BALANCED;
                        m = m->left;
                    }
                    if (m->parent != u) {
                        // m is a successor of u
                        Cell *l = m->parent;
                        if (m->right != nullptr) {
                            m->right->parent = m->parent;
                        }
                        m->parent->left = m->right;

                        m->left = u->left, m->right = u->right;
                        u->left->parent = u->right->parent = m;
                        m->parent = p;
                        *ins = m;
                        rebalanceChain(l, AVL_RIGHT);
                    } else {
                        // m is the right child of u
                        m->left = u->left;
                        u->left->parent = m;
                        *ins = m;
                        rebalanceChain(m, AVL_LEFT);
                    }
                }
                delete u;
                done = true;
            }
        }
    }
}

void AVLTree::rebalanceChain(Cell *p, uint8_t pSide) {
    Cell *head = p, *chainTop = nullptr;
    uint8_t side = pSide;
    // correct 0 0 cells' balances
    while (head->bal == AVL_BALANCED && head->parent != nullptr) {
        head->bal = side;
        side = head->parent->right == head ? AVL_RIGHT : AVL_LEFT;
        chainTop = head;
        head = head->parent;
        // loop ends when chain head is reached
    }
    if (head->parent == nullptr && head->bal == AVL_BALANCED) {
        head->bal = side;
    } else if (head->bal == !side) {
        // x at short branch: already balanced
        head->bal = AVL_BALANCED;
    } else if (head->bal == side) {
        // x at long branch
        if (p == chainTop && head != p && chainTop->bal == !head->bal &&
            (p->left == nullptr || p->right == nullptr) &&
            (head->left == nullptr || head->right == nullptr)) {
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
    Cell *s = a->parent, *p = s->parent;
    Cell *b, *t1, *t2, *t3, *tx;
    if (a->bal == AVL_LEFT) {
        b = a->left;
        t1 = s->left;
        t2 = a->right;
    } else {
        b = a->right;
        t1 = s->right;
        t2 = a->left;
    }
    if (b->bal == AVL_LEFT) {
        tx = b->left;
        t3 = b->right;
    } else {
        tx = b->right;
        t3 = b->left;
    }

    s->parent = b;
    if (a->bal == AVL_LEFT) {
        if (b->bal == AVL_LEFT) {
            s->right = tx, tx->parent = s;
        } else {
            s->right = t3;
            if (t3 != nullptr) t3->parent = s;
        }
    } else {
        if (b->bal == AVL_LEFT) {
            s->left = t3;
            if (t3 != nullptr) t3->parent = s;
        } else {
            s->left = tx, tx->parent = s;
        }
    }

    a->parent = b;
    if (a->bal == AVL_LEFT) {
        if (b->bal == AVL_LEFT) {
            a->left = t3;
            if (t3 != nullptr) t3->parent = a;
        } else {
            a->left = tx, tx->parent = a;
        }
    } else {
        if (b->bal == AVL_LEFT) {
            a->right = tx, tx->parent = a;
        } else {
            a->right = t3;
            if (t3 != nullptr) t3->parent = a;
        }
    }

    b->parent = p;
    if (p == nullptr)
        root = b;
    else if (p->left == s)
        p->left = b;
    else
        p->right = b;
    if (a->bal == AVL_LEFT)
        b->left = s, b->right = a;
    else
        b->left = a, b->right = s;

    if (a->bal == b->bal) {
        a->bal = !b->bal, s->bal = AVL_BALANCED;
    } else {
        a->bal = AVL_BALANCED, s->bal = !b->bal;
    }
    b->bal = AVL_BALANCED;
}

}  // namespace Sealib
