#ifndef SEALIB_COLLECTION_AVLTREE_H_
#define SEALIB_COLLECTION_AVLTREE_H_
#include "sealib/_types.h"

namespace Sealib {
const uint8_t AVL_BALANCED = 11, AVL_LEFT = 0, AVL_RIGHT = 1;

class AVLTree {
 public:
    AVLTree() : root(nullptr) {}

    uint64_t search(uint64_t key) const;

    void insert(uint64_t key, uint64_t data = 0);

    void remove(uint64_t key);

 private:
    struct Cell {
        Cell(uint64_t k, uint64_t v, Cell *p = nullptr)
            : key(k), data(v), parent(p) {}
        uint64_t key;
        uint64_t data;
        Cell *parent = nullptr, *left = nullptr, *right = nullptr;
        uint8_t bal = AVL_BALANCED;
    };

    Cell *root;

    void rebalanceChain(Cell *p, uint8_t side);
    void rebalanceBranch(Cell *p, uint8_t side);
    void rotateTree(Cell *a);
    void spliceTree(Cell *a);
    void swapLeaves(Cell *a);
    void stealChild(Cell *a);
};
}  // namespace Sealib
#endif  // SEALIB_COLLECTION_AVLTREE_H_
