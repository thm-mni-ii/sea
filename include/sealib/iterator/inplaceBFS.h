#ifndef SEALIB_ITERATOR_INPLACEBFS_H_
#define SEALIB_ITERATOR_INPLACEBFS_H_

#include <memory>

#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/graph/compactgraph.h"

#define BFS_WHITE 0
#define BFS_LIGHTGRAY 1
#define BFS_DARKGRAY 2
#define BFS_BLACK 3

typedef void (*UserFunc1)(uint);
typedef void (*UserFunc2)(uint, uint);

#define BFS_NOP_PROCESS (UserFunc1)0
#define BFS_NOP_EXPLORE (UserFunc2)0

/**
 * Run a breadth-first search over a given CompactGraph in shifted
 * representation, while executing the two given user functions. EFFICIENCY:
 * O(n+m) time, O(n+m+2) words
 * @author Simon Schniedenharn
 */

namespace Sealib {
class ChoiceDictionaryDummy : public CompactArray {
 public:
    ChoiceDictionaryDummy(uint n);
    void init(uint color);
    bool more();
    uint next();
    void setColor(uint color, uint v);
    uint color(uint v);

 private:
    uint current_position;
    uint current_color;
    uint size;
};

class InplaceBFS {
 public:
    uint read(uint i);
    /* Creates an InplaceBFS Object with a give Graph and
     * implements a 4-Color Choice Dicrionary inside
     * the shifted graph representations free Space to
     * hold the color information
     * @param g a compactgraph in shifted representation
     * @param color CompactArray as dummy for missing choice dictionary
     * @param pp function pointer for preprocess function
     * @param pe function pointer for postprocess function
     * */
    InplaceBFS(CompactGraph *g, ChoiceDictionaryDummy *color, UserFunc1 pp,
               UserFunc2 pe);

    void runInplaceBFS();

 private:
    CompactGraph *g;
    ChoiceDictionaryDummy *color;
    uint round_number;
    UserFunc1 preprocess;
    UserFunc2 preexplore;
};
}  // namespace Sealib

#endif  // SEALIB_ITERATOR_INPLACEBFS_H_
