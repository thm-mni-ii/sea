#ifndef SEALIB_LEGACY_H_
#define SEALIB_LEGACY_H_
#include <stdint.h>

/**
 * This class provides legacy functions that can be called from C code.
 */

#ifdef __cplusplus
namespace Sealib {
extern "C" {
#endif

/**
 * Create a new graph from an adjacency matrix.
 * @param m Adjacency matrix
 * @param order Size of the matrix
 * @return Pointer to the generated graph
 */
void *Sealib_Graph_new(uint64_t **m, uint64_t order);
void Sealib_Graph_delete(void *self);
/**
 * Generate a random graph.
 * @param order Number of vertices
 * @return Pointer to the generated graph
 */
void *Sealib_Graph_generateRandom(uint64_t order);

/**
 * Create a new choice dictionary.
 * @param size Number of entries
 * @return Pointer to the choice dictionary
 */
void *Sealib_ChoiceDictionary_new(uint64_t size);
void Sealib_ChoiceDictionary_delete(void *self);
/**
 * Set a bit to 1.
 * @param self Choice dictionary instance
 * @param index Index to be written to
 */
void Sealib_ChoiceDictionary_set(void *self, uint64_t index);
/**
 * Get a bit.
 * @param self Choice dictionary instance
 * @param index Index to be read from
 * @returns Bit value
 */
int Sealib_ChoiceDictionary_get(void *self, uint64_t index);
/**
 * Get index of a random set bit.
 * @param self Choice dictionary instance
 * @return Index of a set bit
 */
uint64_t Sealib_ChoiceDictionary_choice(void *self);

/**
 * Create a new choice dictionary iterator.
 * @param choiceDictionary Choice dictionary instance
 * @return Pointer to the choice dictionary iterator
 */
void *Sealib_ChoiceDictionaryIterator_new(void *choiceDictionary);
void Sealib_ChoiceDictionaryIterator_delete(void *choiceDictionary);
/**
 * Initialize the iterator.
 * @param self Choice dictionary iterator instance
 */
void Sealib_ChoiceDictionaryIterator_init(void *self);
/**
 * Check status of the iterator.
 * @param self Choice dictionary iterator instance
 * @return 1 if there are more set bits, 0 otherwise
 */
int Sealib_ChoiceDictionaryIterator_more(void *self);
/**
 * Get next set bit's index.
 * @param self Choice dictionary iterator instance
 * @return Index of the next set bit
 */
uint64_t Sealib_ChoiceDictionaryIterator_next(void *self);

/**
 * Create a new bitset.
 * @param size Number of entries
 * @return Pointer to the bitset
 */
void *Sealib_Bitset_new(uint64_t size);
void Sealib_Bitset_delete(void *self);
/**
 * Set a bit to 1.
 * @param self bitset instance
 * @param index Index to be written to
 */
void Sealib_Bitset_set(void *self, uint64_t index);
/**
 * Get a bit.
 * @param self bitset instance
 * @param index Index to be read from
 * @return Bit value
 */
int Sealib_Bitset_get(void *self, uint64_t index);

/**
 * Create a new rank-select structure
 * @param bitset bitset to use
 * @return Pointer to the rank-select structure
 */
void *Sealib_RankSelect_new(void *bitset);
void Sealib_RankSelect_delete(void *self);
/**
 * Get the rank of an index.
 * @param self rank-select instance
 * @param index Index to get the rank for
 * @return Rank (no. set bits up to the index)
 */
uint64_t Sealib_RankSelect_rank(void *self, uint64_t index);
/**
 * Select a bit.
 * @param self rank-select instance
 * @param bit Which set bit to select
 * @return Index of the `bit`-th set bit
 */
uint64_t Sealib_RankSelect_select(void *self, uint64_t bit);
/**
 * Get the size.
 * @param self rank-select instance
 * @return Number of entries
 */
uint64_t Sealib_RankSelect_size(void *self);

/**
 * Run a DFS in O(n log(log(n))) bits.
 * @param graph Graph instance
 * @param preprocess user procedure to execute before processing a node u
 * @param preexplore user procedure to execute before exploring an edge u,v
 * @param postexplore user procedure to execute before exploring an edge u,v
 * @param postprocess user procedure to execute before processing a node u
 */
void Sealib_DFS_nloglognBitDFS(void *graph, void (*preprocess)(uint64_t),
                               void (*preexplore)(uint64_t, uint64_t),
                               void (*postexplore)(uint64_t, uint64_t),
                               void (*postprocess)(uint64_t));

#ifdef __cplusplus
}
}  // namespace Sealib
#endif
#endif  // SEALIB_LEGACY_H_
