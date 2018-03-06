//
// Created by jmeintrup on 06.03.18.
//

#include "sealib/crosslinkedint.h"

/**
 * Constructor of MatchedInt.
 * Constructs it with a nullptr as it's match.
 * @param _v value of the matched int.
 */
CrossLinkedInt::CrossLinkedInt(int _v)  {
    v = _v;
    m = nullptr;
}

/**
 * Sets the match as a crosspointer to _m, and from _m to this.
 * @param _m MatchedInt object to be matched with.
 */
void CrossLinkedInt::match(CrossLinkedInt *_m) {
    m = _m;
    m->set_match(this);
}

void CrossLinkedInt::set_match(CrossLinkedInt *_m) {
    m = _m;
}

/**
 * Getter for the value.
 * @return v
 */
int CrossLinkedInt::get_value() {
    return v;
}

/**
 * Getter for the match.
 * @return m
 */
CrossLinkedInt* CrossLinkedInt::get_match() {
    return m;
}

/**
 * Checks whether or not this has a match.
 * @return true if m is not equal to nullptr
 */
bool CrossLinkedInt::has_match() {
    return m != nullptr;
}

/**
 * Unmatches this.
 * Sets m->m to nullptr, and m to nullptr.
 */
void CrossLinkedInt::unmatch() {
    m->set_match(nullptr);
    m = nullptr;
}

