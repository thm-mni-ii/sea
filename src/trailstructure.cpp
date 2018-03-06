//
// Created by jmeintrup on 06.03.18.
//

#include "sealib/trailstructure.h"

/**
 * Function to check wheter U is an empty set.
 * Serves the double purpose of checking if the trailstructure is in phase two.
 * @return true if the set U is empty, otherwise false
 */
bool TrailStructure::is_u_empty() {
    return false;
}

/**
 * Checks whether or not a MatchedInt *d in I U or O is matched.
 * @param MatchedInt *d, which has to be in I U or O.
 * @return true if the MatchedInt d has a match, otherwise false.
 */
bool TrailStructure::is_matched(MatchedInt *d) {
    if(!(is_in_i(d) || is_in_o(d))) {
        throw std::invalid_argument("Matched int d is not in I or O!");
    }
    MatchedInt *m = d->get_match();
    if(m == nullptr) return false;

    if(!(is_in_i(m) || is_in_o(m))) {
        throw std::invalid_argument("Matched int m is not in I or O!");
    }

    return true;
}

/**
 * Checks whether or not a MatchedInt *d in I.
 * @param MatchedInt *d
 * @return true if the MatchedInt d is in I, otherwise false.
 */
bool TrailStructure::is_in_i(MatchedInt *d) {
    return i.count(d) == 1;
}

/**
 * Checks whether or not a MatchedInt *d in O.
 * @param MatchedInt *d
 * @return true if the MatchedInt d is in O, otherwise false.
 */
bool TrailStructure::is_in_o(MatchedInt *d) {
    return o.count(d) == 1;;
}

/**
 * Checks whether or not a MatchedInt *d in O.
 * @param MatchedInt *d
 * @return true if the MatchedInt d is in O, otherwise false.
 */
bool TrailStructure::is_in_u(MatchedInt *d) {
    return u.count(d) == 1;;
}

/**
 * Selects an arbitrary element of U, moves it from U to O, leaves it unmatched and returns it.
 * @param MatchedInt *d
 * @return arbitrary element .
 */
MatchedInt* TrailStructure::leave() {
    if(is_u_empty()) throw PhaseException();
    MatchedInt* d = *std::next(u.begin());
    u.erase(d);
    o.insert(d);
    return  d;
}

/**
 * Takes as argument MatchedInt *d wich has to be in U. Moves it from U to O.
 * Afterwards, if U is not empty, takes an arbitrary element mi from U, moves it to I.
 * Links that element with *d and returns it.
 * If U is empty, returns nullptr.
 * @param MatchedInt *d
 * @return Linked MatchedInt *mi or nulltptr.
 */
MatchedInt* TrailStructure::enter(MatchedInt *d) {
    if(!is_in_u(d)) throw std::invalid_argument("MatchedInt d is not in U!");
    u.erase(d);
    i.insert(d);

    if(!is_u_empty()) {
        MatchedInt* mi = *std::next(u.begin());
        u.erase(mi);
        o.insert(mi);
        d->match(mi);
        return mi;
    } else {
        return nullptr;
    }
}

/**
 * Function to get the match of an element in I or O.
 * Can only be called in phase two. (when u is empty).
 * @param d MatchedInt* in I or O.
 * @return The match of d.
 */
MatchedInt* TrailStructure::get_matched(MatchedInt *d) {
    if(!is_u_empty()) throw PhaseException();
    if(!(is_in_i(d) || is_in_o(d))) throw std::invalid_argument("MatchedInt d is not in I or O!");

    if(d->get_match() == nullptr) throw std::invalid_argument("MatchedInt d has no match!");

    return d->get_match();
}

/**
 * Matched an element of I with an element of O.
 * Unmatches them first.
 * Can only be called in phase two.
 * mi has to be an element of I, and mo has to be an element of O.
 * @param mi element of I
 * @param mo element of O
 */
void TrailStructure::marry(MatchedInt *mi, MatchedInt *mo) {
    if(!is_u_empty()) throw PhaseException();
    if(!is_in_i(mi)) throw std::invalid_argument("MatchedInt mi not in I!");
    if(!is_in_i(mo)) throw std::invalid_argument("MatchedInt mo not in O!");

    mi->unmatch();
    mo->unmatch();
    mi->match(mo);
}
/**
 * Adds a new MatchedInt to U.
 * This function is to be used during the creation of the graph.
 * You should not add extra elements to U during a run of the algorithm.
 * @param d MatchedInt to be added to U
 */

void TrailStructure::add_to_u(MatchedInt *d) {
    u.insert(d);
}

/**
 * Constructor.
 */
TrailStructure::TrailStructure() {
}

std::unordered_set<MatchedInt *> *TrailStructure::get_u() {
    return &u;
}
