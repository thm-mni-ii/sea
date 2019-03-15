#ifndef SEALIB_COLLECTION_AVLTREE_H_
#define SEALIB_COLLECTION_AVLTREE_H_
#include "sealib/_types.h"

namespace Sealib {
const uint8_t AVL_BALANCED = 11, AVL_LEFT = 0, AVL_RIGHT = 1;

/**
 * A self-balancing binary tree with O(log(n)) time for search, insertion and
 * deletion. (Adel'son-Vel'skii, Landis; 1962)
 *
 * @author Simon Heuser
 */
class AVLTree {
 public:
    /**
     * Construct an empty AVL tree.
     */
    AVLTree() : root(nullptr) {}

    /**
     * Search for data stored at the given node.
     * @param key The key to search for
     * @return Data stored at the node, or INVALID if the node was not found
     */
    uint64_t search(uint64_t key) const;

    /**
     * Insert a new node with the given key and data.
     * If the node already exists, it is updated with the given data.
     * @param key Key of the node to be inserted
     * @param data Data to insert for the node
     */
    void insert(uint64_t key, uint64_t data = 0);

    /**
     * Remove a node from the tree.
     * @param key Key of the node to be removed
     */
    void remove(uint64_t key);

    // The AVLTree class may be moved, but not copied.
    AVLTree(AVLTree const &) = delete;
    AVLTree &operator=(AVLTree const &) = delete;
    AVLTree(AVLTree &&) = default;
    AVLTree &operator=(AVLTree &&) = default;
    ~AVLTree();

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
    void swapLeaves(Cell *a);
    void rotateTree(Cell *a);
    void spliceTree(Cell *a);
};
}  // namespace Sealib
#endif  // SEALIB_COLLECTION_AVLTREE_H_
