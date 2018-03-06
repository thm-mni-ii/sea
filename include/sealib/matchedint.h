//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_MATCHEDINT_H
#define SEA_MATCHEDINT_H

/**
 * Helper Class for TrailStructure.
 * Contains a value and a crosspointer to a matched MatchedInt object.
 */
class MatchedInt{
private:
    int v;
    MatchedInt *m;
    void set_match(MatchedInt *m);

public:
    MatchedInt(int v);
    void match(MatchedInt *m);
    void unmatch();
    int get_value();
    MatchedInt* get_match();
    bool has_match();
};
#endif //SEA_MATCHEDINT_H
