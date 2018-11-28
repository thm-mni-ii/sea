#ifndef SEALIB_LEGACY_H_
#define SEALIB_LEGACY_H_
#include <stdint.h>
#ifdef __cplusplus
namespace Sealib {
extern "C" {
#endif

void *Sealib_Graph_new(uint32_t **m, uint32_t order);
void Sealib_Graph_delete(void *self);
void *Sealib_Graph_generateRandom(uint32_t order);

void *Sealib_ChoiceDictionary_new(uint32_t size);
void Sealib_ChoiceDictionary_delete(void *self);
void Sealib_ChoiceDictionary_set(void *self, uint64_t index);
int Sealib_ChoiceDictionary_get(void *self, uint64_t index);
uint64_t Sealib_ChoiceDictionary_choice(void *self);

void *Sealib_ChoiceDictionaryIterator_new(void *choiceDictionary);
void Sealib_ChoiceDictionaryIterator_delete(void *choiceDictionary);
void Sealib_ChoiceDictionaryIterator_init(void *self);
int Sealib_ChoiceDictionaryIterator_more(void *self);
uint64_t Sealib_ChoiceDictionaryIterator_next(void *self);

void *Sealib_Bitset_new(uint64_t size);
void Sealib_Bitset_delete(void *self);
void Sealib_Bitset_set(void *self, uint64_t index);
int Sealib_Bitset_get(void *self, uint64_t index);

void *Sealib_RankSelect_new(void *bitset);
void Sealib_RankSelect_delete(void *self);
uint64_t Sealib_RankSelect_rank(void *self, uint64_t index);
uint64_t Sealib_RankSelect_select(void *self, uint64_t bit);
uint64_t Sealib_RankSelect_size(void *self);

void Sealib_DFS_nloglognBitDFS(void *graph, void (*preprocess)(uint32_t),
                               void (*preexplore)(uint32_t, uint32_t),
                               void (*postexplore)(uint32_t, uint32_t),
                               void (*postprocess)(uint32_t));

#ifdef __cplusplus
}
}  // namespace Sealib
#endif
#endif  // SEALIB_LEGACY_H_
