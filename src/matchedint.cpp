//
// Created by jmeintrup on 06.03.18.
//

#include "sealib/matchedint.h"

/**
 * Constructor of MatchedInt.
 * Constructs it with a nullptr as it's match.
 * @param _v value of the matched int.
 */
MatchedInt::MatchedInt(int _v)  {
    v = _v;
    m = nullptr;
}

/**
 * Sets the match as a crosspointer to _m, and from _m to this.
 * @param _m MatchedInt object to be matched with.
 */
void MatchedInt::match(MatchedInt *_m) {
    m = _m;
    m->set_match(this);
}

void MatchedInt::set_match(MatchedInt *_m) {
    m = _m;
}

/**
 * Getter for the value.
 * @return v
 */
int MatchedInt::get_value() {
    return v;
}

/**
 * Getter for the match.
 * @return m
 */
MatchedInt* MatchedInt::get_match() {
    return m;
}

/**
 * Checks whether or not this has a match.
 * @return true if m is not equal to nullptr
 */
bool MatchedInt::has_match() {
    return m != nullptr;
}

/**
 * Unmatches this.
 * Sets m->m to nullptr, and m to nullptr.
 */
void MatchedInt::unmatch() {
    m->set_match(nullptr);
    m = nullptr;
}

