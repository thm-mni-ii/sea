//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_TRAILSTRUCTURE_H
#define SEA_TRAILSTRUCTURE_H

#include <vector>

/**
 * Space efficient TrailStructure.
 * Each vertex in a graph has an object of this type to store trail information.
 */
class TrailStructure{
private:
    size_t degree;
    size_t currDeg;
    size_t nextUnused;

    std::vector<bool> inAndOut;
    std::vector<bool> matched;
    size_t *unused;

public:
    explicit TrailStructure(size_t _degree);
    size_t getCurrDeg();
    bool isUnusedEmpty();
    bool isMatched(size_t idx);
    size_t getNextUnused(size_t degree);

//    CrossLinkedSizeT* leave();
//    CrossLinkedSizeT* enter(CrossLinkedSizeT *d);
    size_t getMatched(size_t start, size_t idx);
//    void marry(CrossLinkedSizeT *mi, CrossLinkedSizeT *mo);
};

#endif //SEA_TRAILSTRUCTURE_H
