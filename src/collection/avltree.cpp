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
                    rebalanceParents(u, AVL_LEFT);
                    done = true;
                } else {
                    u = u->left;
                }
            } else if (k > u->key) {
                if (u->right == nullptr) {
                    u->right = new Cell(k, v, u);
                    rebalanceParents(u, AVL_LEFT);
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

void AVLTree::rebalanceParents(Cell *p, uint8_t side) {
    uint8_t other = side = AVL_LEFT ? AVL_RIGHT : AVL_LEFT;
    if (p->bal == other) {
        p->bal = AVL_BALANCED;
    } else {
        // correct 0 0 chain to the side:
        Cell *onChain = p;
        while (onChain->bal == AVL_BALANCED && onChain->parent != nullptr) {
            onChain->bal = side;
            onChain = onChain->parent;
            // loop ends when chain pivot is reached
        }
        Cell *chainPivot = onChain;
        if (chainPivot->bal == other) {
            // inserted at short branch => already balanced
            chainPivot->bal = AVL_BALANCED;
        } else {
            // inserted at long branch => rebalance
            if (chainPivot->bal == other &&
                (chainPivot->left == nullptr || chainPivot->right == nullptr)) {
                Cell *child = chainPivot->left != nullptr ? chainPivot->left
                                                          : chainPivot->right;
                if (child->left == nullptr || child->right == nullptr) {
                    rebalanceLongLine(child);
                }
            } else if (chainPivot->parent != nullptr) {
                Cell *root = chainPivot->parent;
                if (root->bal == side) {
                    rebalanceDoubleHeavy(chainPivot);
                } else if (root->bal == other) {
                    rebalanceSideSwitching(chainPivot);
                }
            }
        }
    }
}

void AVLTree::rebalanceLongLine(Cell *pivot) {
    Cell *x = pivot->left == nullptr ? pivot->right : pivot->left;
    Cell *s = pivot->parent;
    x->parent = s->parent;
    s->parent = pivot->parent = x;
    s->left = s->right = nullptr;
    pivot->left = pivot->right = nullptr;
    s->bal = pivot->bal = AVL_BALANCED;
}

void AVLTree::rebalanceDoubleHeavy(Cell *) {}
void AVLTree::rebalanceSideSwitching(Cell *) {}

}  // namespace Sealib