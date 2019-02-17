#include "sealib/legacy.h"
#include <vector>
#include "sealib/collection/bitset.h"
#include "sealib/dictionary/rankselect.h"
#include "sealib/graph/graphcreator.h"
#include "sealib/iterator/choicedictionaryiterator.h"
#include "sealib/iterator/dfs.h"

namespace Sealib {

void *Sealib_Graph_new(uint64_t **m, uint64_t order) {
    return GraphCreator::createPointerFromAdjacencyMatrix(m, order);
}
void Sealib_Graph_delete(void *self) {
    delete static_cast<UndirectedGraph const *>(self);
}
void *Sealib_Graph_generateRandom(uint64_t order) {
    std::vector<SimpleNode> n(order);
    static std::random_device rng;
    std::uniform_int_distribution<uint64_t> rnd(0, order - 1);
    for (uint64_t a = 0; a < order; a++) {
        uint64_t deg = rnd(rng);
        std::vector<uint64_t> ad;
        for (uint64_t b = 0; b < deg; b++) {
            ad.emplace_back(rnd(rng));
        }
        n[a] = SimpleNode(ad);
    }
    return new DirectedGraph(n);
}

void *Sealib_ChoiceDictionary_new(uint64_t size) {
    return new ChoiceDictionary(size);
}
void Sealib_ChoiceDictionary_delete(void *self) {
    delete static_cast<ChoiceDictionary *>(self);
}
void Sealib_ChoiceDictionary_set(void *self, uint64_t index) {
    static_cast<ChoiceDictionary *>(self)->insert(index);
}
int Sealib_ChoiceDictionary_get(void *self, uint64_t index) {
    return static_cast<ChoiceDictionary *>(self)->get(index);
}
uint64_t Sealib_ChoiceDictionary_choice(void *self) {
    return static_cast<ChoiceDictionary *>(self)->choice();
}

void *Sealib_ChoiceDictionaryIterator_new(void *choiceDictionary) {
    return new ChoiceDictionaryIterator(
        *static_cast<ChoiceDictionary *>(choiceDictionary));
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
uint64_t Sealib_ChoiceDictionaryIterator_next(void *self) {
    return static_cast<ChoiceDictionaryIterator *>(self)->next();
}

void *Sealib_Bitset_new(uint64_t size) { return new Bitset<uint8_t>(size); }
void Sealib_Bitset_delete(void *self) {
    delete static_cast<Bitset<uint8_t> *>(self);
}
void Sealib_Bitset_set(void *self, uint64_t index) {
    Bitset<uint8_t> &b = *static_cast<Bitset<uint8_t> *>(self);
    b[index] = 1;
}
int Sealib_Bitset_get(void *self, uint64_t index) {
    return static_cast<Bitset<uint8_t> *>(self)->get(index);
}

void *Sealib_RankSelect_new(void *bitset) {
    return new RankSelect(*static_cast<Bitset<uint8_t> *>(bitset));
}
void Sealib_RankSelect_delete(void *self) {
    delete static_cast<RankSelect *>(self);
}
uint64_t Sealib_RankSelect_rank(void *self, uint64_t index) {
    return static_cast<RankSelect *>(self)->rank(index);
}
uint64_t Sealib_RankSelect_select(void *self, uint64_t bit) {
    return static_cast<RankSelect *>(self)->select(bit);
}
uint64_t Sealib_RankSelect_size(void *self) {
    return static_cast<RankSelect *>(self)->size();
}

void Sealib_DFS_nloglognBitDFS(void *graph, void (*preprocess)(uint64_t),
                               void (*preexplore)(uint64_t, uint64_t),
                               void (*postexplore)(uint64_t, uint64_t),
                               void (*postprocess)(uint64_t)) {
    if (preprocess == nullptr) preprocess = [](uint64_t) {};
    if (preexplore == nullptr) preexplore = [](uint64_t, uint64_t) {};
    if (postexplore == nullptr) postexplore = [](uint64_t, uint64_t) {};
    if (postprocess == nullptr) postprocess = [](uint64_t) {};
    DFS::nloglognBitDFS(*static_cast<Graph const *>(graph), preprocess,
                        preexplore, postexplore, postprocess);
}
}  // namespace Sealib
