#include "sealib/legacy.h"
#include "sealib/bitset.h"
#include "sealib/choicedictionaryiterator.h"
#include "sealib/dfs.h"
#include "sealib/graphcreator.h"
#include "sealib/rankselect.h"

using namespace Sealib;  // NOLINT

void *Sealib_Graph_new(unsigned int **m, unsigned int order) {
  return GraphCreator::createGraphPointerFromAdjacencyMatrix(m, order);
}
void Sealib_Graph_delete(void *self) { delete static_cast<Graph *>(self); }
void *Sealib_Graph_generateRandom(unsigned int order) {
  return GraphCreator::createRandomGenerated(order);
}

void *Sealib_ChoiceDictionary_new(unsigned int size) {
  return new ChoiceDictionary(size);
}
void Sealib_ChoiceDictionary_delete(void *self) {
  delete static_cast<ChoiceDictionary *>(self);
}
void Sealib_ChoiceDictionary_set(void *self, unsigned long index) {
  static_cast<ChoiceDictionary *>(self)->insert(index);
}
int Sealib_ChoiceDictionary_get(void *self, unsigned long index) {
  return static_cast<ChoiceDictionary *>(self)->get(index);
}
unsigned long Sealib_ChoiceDictionary_choice(void *self) {
  return static_cast<ChoiceDictionary *>(self)->choice();
}

void *Sealib_ChoiceDictionaryIterator_new(void *choiceDictionary) {
  return new ChoiceDictionaryIterator(
      static_cast<ChoiceDictionary *>(choiceDictionary));
}
void Sealib_ChoiceDictionaryIterator_delete(void *self) {
  delete static_cast<ChoiceDictionaryIterator *>(self);
}
void Sealib_ChoiceDictionaryIterator_init(void *self) {
  static_cast<ChoiceDictionaryIterator *>(self)->init();
}
int Sealib_ChoiceDictionaryIterator_more(void *self) {
  return static_cast<ChoiceDictionaryIterator *>(self)->more();
}
unsigned long Sealib_ChoiceDictionaryIterator_next(void *self) {
  return static_cast<ChoiceDictionaryIterator *>(self)->next();
}

void *Sealib_Bitset_new(unsigned long size) {
  return new Bitset<unsigned char>(size);
}
void Sealib_Bitset_delete(void *self) {
  delete static_cast<Bitset<unsigned char> *>(self);
}
void Sealib_Bitset_set(void *self, unsigned long index) {
  Bitset<unsigned char> &b = *static_cast<Bitset<unsigned char> *>(self);
  b[index] = 1;
}
int Sealib_Bitset_get(void *self, unsigned long index) {
  return static_cast<Bitset<unsigned char> *>(self)->get(index);
}

void *Sealib_RankSelect_new(void *bitset) {
  return new RankSelect(*static_cast<Bitset<unsigned char> *>(bitset));
}
void Sealib_RankSelect_delete(void *self) {
  delete static_cast<RankSelect *>(self);
}
unsigned long Sealib_RankSelect_rank(void *self, unsigned long index) {
  return static_cast<RankSelect *>(self)->rank(index);
}
unsigned long Sealib_RankSelect_select(void *self, unsigned long bit) {
  return static_cast<RankSelect *>(self)->select(bit);
}
unsigned long Sealib_RankSelect_size(void *self) {
  return static_cast<RankSelect *>(self)->size();
}

void Sealib_DFS_nloglognBitDFS(void *graph, void (*preprocess)(unsigned int),
                               void (*preexplore)(unsigned int, unsigned int),
                               void (*postexplore)(unsigned int, unsigned int),
                               void (*postprocess)(unsigned int)) {
    if (preprocess == nullptr) preprocess = [](uint){};
    if (preexplore == nullptr) preexplore = [](uint, uint){};
    if (postexplore == nullptr) postexplore = [](uint, uint){};
    if (postprocess == nullptr) postprocess = [](uint){};
    DFS::nloglognBitDFS(static_cast<Graph *>(graph), preprocess, preexplore,
                        postexplore, postprocess);
}
