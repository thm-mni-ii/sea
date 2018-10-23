#ifndef SEALIB_LEGACY_H_
#define SEALIB_LEGACY_H_
#ifdef __cplusplus
extern "C" {
#endif

void *Sealib_Graph_new(unsigned int **m, unsigned int order);
void Sealib_Graph_delete(void *self);
void *Sealib_Graph_generateRandom(unsigned int order);

void *Sealib_ChoiceDictionary_new(unsigned int size);
void Sealib_ChoiceDictionary_delete(void *self);
void Sealib_ChoiceDictionary_set(void *self, unsigned long index);
int Sealib_ChoiceDictionary_get(void *self, unsigned long index);
unsigned long Sealib_ChoiceDictionary_choice(void *self);

void *Sealib_ChoiceDictionaryIterator_new(void *choiceDictionary);
void Sealib_ChoiceDictionaryIterator_delete(void *choiceDictionary);
void Sealib_ChoiceDictionaryIterator_init(void *self);
int Sealib_ChoiceDictionaryIterator_more(void *self);
unsigned long Sealib_ChoiceDictionaryIterator_next(void *self);

void *Sealib_Bitset_new(unsigned long size);
void Sealib_Bitset_delete(void *self);
void Sealib_Bitset_set(void *self, unsigned long index);
int Sealib_Bitset_get(void *self, unsigned long index);

void *Sealib_RankSelect_new(void *bitset);
void Sealib_RankSelect_delete(void *self);
unsigned long Sealib_RankSelect_rank(void *self, unsigned long index);
unsigned long Sealib_RankSelect_select(void *self, unsigned long bit);
unsigned long Sealib_RankSelect_size(void *self);

void Sealib_DFS_nloglognBitDFS(void *graph, void (*preprocess)(unsigned int),
                               void (*preexplore)(unsigned int, unsigned int),
                               void (*postexplore)(unsigned int, unsigned int),
                               void (*postprocess)(unsigned int));

#ifdef __cplusplus
}
#endif
#endif  // SEALIB_LEGACY_H_
