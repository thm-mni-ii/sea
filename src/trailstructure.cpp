//
// Created by jmeintrup on 06.03.18.
//

#include <cstdlib>
#include "sealib/trailstructure.h"
using namespace std;

TrailStructure::TrailStructure(size_t _degree) : currDeg(_degree),degree(degree){
    nextUnused = 1;

    inAndOut = vector<bool>(degree);
    matched = vector<bool>(degree);
    unused = static_cast<size_t *>(malloc(sizeof(size_t) * degree * 3));


    //[1][0][1] [1][1][1] [1][2][1] ... [1][_degree-1][1]
    for(size_t i = 0; i < degree; i+=3) {
        unused[i] = 1;
        unused[i + 1] = i/3;
        unused[i + 2] = 1;
    }
}

size_t TrailStructure::getCurrDeg() {
    return currDeg;
}


/**
 * If unused is empty, nextUnused is set to 0;
 * @return true if unused is not empty, false otherwise.
 */
bool TrailStructure::isUnusedEmpty() {
    return (nextUnused == 0);
}

/**
 * Checks if the arc at index idx is matched.
 * Internally, checks if the bit at index idx is set in the bool vector
 * of matches.
 * @param idx to be checked at
 * @return true if matches, false otherwise
 */
bool TrailStructure::isMatched(size_t idx) {
    return matched.at(idx);
}

/**
 * Returns the next unused element
 * @return
 */
size_t TrailStructure::getNextUnused(size_t degree) {
    if(nextUnused == 0) return 0;
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
        currDeg--; //should be 0 now
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
    currDeg--;

    return retVal;
}

size_t TrailStructure::getMatched(size_t start, size_t idx) {
    if(!matched[idx]) return idx; //has no match

    //get start idx for the dyck word
    size_t j = start;
    size_t p = 0;
    size_t stack[(degree - currDeg)/2];
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
        if(j = degree - 1) {
            j = 0;
        } else {
            j++;
        }
    } while(j != start);

    return idx;
}


