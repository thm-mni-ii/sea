//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_MATCHEDINT_H
#define SEA_MATCHEDINT_H

#include <cstddef>

/**
 * Helper Class for TrailStructure.
 * Contains a value and a crosspointer to a matched MatchedInt object.
 */
class CrossLinkedSizeT{
private:
    size_t v;
    CrossLinkedSizeT *m;
    void set_match(CrossLinkedSizeT *m);

public:
    CrossLinkedSizeT(size_t v);
    void match(CrossLinkedSizeT *m);
    void unmatch();
    size_t get_value();
    CrossLinkedSizeT* get_match();
    bool has_match();
};
#endif //SEA_MATCHEDINT_H
