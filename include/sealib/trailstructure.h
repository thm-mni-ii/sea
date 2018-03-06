//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_TRAILSTRUCTURE_H
#define SEA_TRAILSTRUCTURE_H

#include <unordered_set>
#include "matchedint.h"
#include <stdexcept>

/**
 * Space efficient TrailStructure.
 * Each vertex in a graph has an object of this type to store trail information.
 */
class TrailStructure{
private:
    std::unordered_set<MatchedInt*> i;
    std::unordered_set<MatchedInt*> o;
    std::unordered_set<MatchedInt*> u;

public:
    TrailStructure();
    std::unordered_set<MatchedInt*> *get_u();
    void add_to_u(MatchedInt* d);
    bool is_u_empty();
    bool is_matched(MatchedInt *d);
    bool is_in_i(MatchedInt *d);
    bool is_in_o(MatchedInt *d);
    bool is_in_u(MatchedInt *d);
    MatchedInt* leave();
    MatchedInt* enter(MatchedInt *d);
    MatchedInt* get_matched(MatchedInt *d);
    void marry(MatchedInt *mi, MatchedInt *mo);
};

class PhaseException : public std::exception {
    const char* what() const throw() { return "TrailStructure is in the wrong phase for this function\n"; }
};

#endif //SEA_TRAILSTRUCTURE_H
