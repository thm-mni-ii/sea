//
// Created by jmeintrup on 06.03.18.
//

#include <cstdlib>
#include "sealib/trailstructure.h"
using namespace std;

TrailStructure::TrailStructure(size_t _degree) : degree(degree){
    nextUnused = 1;
    lastMatchedLeaver = (size_t) - 1;

    inAndOut = vector<bool>(degree);
    matched = vector<bool>(degree);
    flags = std::vector<bool>(2);
    if(degree % 2 != 0) flags.at(0).flip(); //set it to grey if uneven
    if(degree == 0) flags.at(1).flip(); //node with no edges is possible, set black

    unused = static_cast<size_t *>(malloc(sizeof(size_t) * degree * 3));


    //[1][0][1] [1][1][1] [1][2][1] ... [1][_degree-1][1]
    for(size_t i = 0; i < degree; i+=3) {
        unused[i] = 1;
        unused[i + 1] = i/3;
        unused[i + 2] = 1;
    }
}

/**
 * Returns the next unused element
 * @return
 */
inline size_t TrailStructure::getNextUnused() {
    if(flags.at(1)) return 0; //black node
    size_t prevLink = unused[nextUnused-1];
    size_t nextLink = unused[nextUnused+1];

    size_t temp;

    if(prevLink*3 > nextUnused) { //circle around
        temp = (degree * 3) - (prevLink * 3) + nextUnused;
    } else {
        temp = nextUnused - prevLink * 3;
    }
    if(temp == nextUnused) { //no other element, this is last
        size_t retVal = nextUnused;
        nextUnused = 0;
        flags.at(1).flip();
        return retVal;
    }
    unused[temp+1] += nextLink;

    if(nextLink*3 + nextUnused > degree) { //circle around
        temp = (nextLink * 3) - (degree * 3)   + nextUnused;
    } else {
        temp = nextUnused - nextLink * 3;
    }
    unused[temp-1] += prevLink;
    size_t retVal = nextUnused;
    nextUnused = temp;
    flags.at(0).flip(); //taking an arc flips the parity

    return retVal;
}

size_t TrailStructure::getMatched(size_t start, size_t idx) {
    if(!matched[idx]) return idx; //has no match

    //get start idx for the dyck word
    size_t j = start;
    size_t p = 0;
    size_t stack[degree/2];
    do {
        if(matched[j]) { //only push matched index
            if(inAndOut[j]) { // '('
                stack[p++] = j;
            } else {
                size_t i = stack[--p];
                if(idx == i) return j;
                if(idx == j) return i;
            }
        }

        //increment circular
        if(j == degree - 1) {
            j = 0;
        } else {
            j++;
        }
    } while(j != start);

    return idx;
}

size_t TrailStructure::leave() {
    size_t u = getNextUnused();
    return u == 0 ? (size_t) - 1 : unused[u];
}

size_t TrailStructure::enter(size_t i) {
    i = i*3+1; //multiple index so it works with the actual array.

    if(flags.at(1)) return (size_t) - 1;

    size_t prevLink = unused[i-1];
    size_t nextLink = unused[i+1];

    size_t temp;

    if(prevLink*3 > i) { //circle around
        temp = (degree * 3) - (prevLink * 3) + i;
    } else {
        temp = i - prevLink * 3;
    }
    if(temp == i) { //no other element, this is last
        flags.at(1).flip(); //should be 0 now
        return (size_t) - 1; //returns non-value
    }
    unused[temp+1] += nextLink;

    if(nextLink*3 + i > degree) { //circle around
        temp = (nextLink * 3) - (degree * 3)   + i;
    } else {
        temp = i - nextLink * 3;
    }
    unused[temp-1] += prevLink;

    //not needed, we flip twice since we take another edge out now
    //flags.at(0).flip(); //taking an arc flips the parity

    //not empty yet, continue
    matched.at(i).flip();
    matched.at(unused[temp]).flip();

    i = temp;

    prevLink = unused[i-1];
    nextLink = unused[i+1];

    if(prevLink*3 > i) { //circle around
        temp = (degree * 3) - (prevLink * 3) + i;
    } else {
        temp = i - prevLink * 3;
    }
    if(temp == i) { //no other element, this is last
        flags.at(1).flip(); //should be 0 now
        return i; //returns the leaver element
    }
    unused[temp+1] += nextLink;

    if(nextLink*3 + i > degree) { //circle around
        temp = (nextLink * 3) - (degree * 3)   + i;
    } else {
        temp = i - nextLink * 3;
    }
    unused[temp-1] += prevLink;

    return i; //returns the leaver element.
}

inline bool TrailStructure::isBlack() {
    return flags.at(1);
}

inline bool TrailStructure::isGrey() {
    return flags.at(0);
}


