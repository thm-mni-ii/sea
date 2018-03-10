//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_MATCHEDINT_H
#define SEA_MATCHEDINT_H

/**
 * Helper Class for TrailStructure.
 * Contains a value and a crosspointer to a matched MatchedInt object.
 */
class CrossLinkedInt{
private:
    int v;
    CrossLinkedInt *m;
    void set_match(CrossLinkedInt *m);

public:
    CrossLinkedInt(int v);
    void match(CrossLinkedInt *m);
    void unmatch();
    int get_value();
    CrossLinkedInt* get_match();
    bool has_match();
};
#endif //SEA_MATCHEDINT_H
