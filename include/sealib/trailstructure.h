//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_TRAILSTRUCTURE_H
#define SEA_TRAILSTRUCTURE_H

#include <unordered_set>
#include "crosslinkedint.h"
#include <stdexcept>

/**
 * Space efficient TrailStructure.
 * Each vertex in a graph has an object of this type to store trail information.
 */
class TrailStructure{
private:
    std::unordered_set<CrossLinkedInt*> i;
    std::unordered_set<CrossLinkedInt*> o;
    std::unordered_set<CrossLinkedInt*> u;

public:
    TrailStructure();
    std::unordered_set<CrossLinkedInt*> *get_u();
    int curr_deg();
    void add_to_u(CrossLinkedInt* d);
    bool is_u_empty();
    bool is_matched(CrossLinkedInt *d);
    bool is_in_i(CrossLinkedInt *d);
    bool is_in_o(CrossLinkedInt *d);
    bool is_in_u(CrossLinkedInt *d);
    CrossLinkedInt* leave();
    CrossLinkedInt* enter(CrossLinkedInt *d);
    CrossLinkedInt* get_matched(CrossLinkedInt *d);
    void marry(CrossLinkedInt *mi, CrossLinkedInt *mo);
};

class PhaseException : public std::exception {
    const char* what() const throw() { return "TrailStructure is in the wrong phase for this function\n"; }
};

#endif //SEA_TRAILSTRUCTURE_H
