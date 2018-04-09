//
// Created by jmeintrup on 06.03.18.
//

#include <cstdlib>
#include "sealib/trailstructure.h"
using namespace std;


TrailStructure::TrailStructure(unsigned int _degree) : degree(degree){
    nextUnused = 1;
    lastMatchedLeaver = (unsigned int) - 1;

    inAndOut = vector<bool>(degree);
    matched = vector<bool>(degree);

    flags = std::vector<bool>(3);
    if(degree % 2 != 0) flags.at(2).flip(); //set it to grey if uneven
    if(degree == 0) flags.at(1).flip(); //node with no edges is possible, set black

    married = static_cast<unsigned int  *>(malloc(sizeof(unsigned int) * 4));
    for(unsigned int i = 0; i < 4; i++) married[i] = (unsigned int) - 1;

    unused = static_cast<unsigned int *>(malloc(sizeof(unsigned int) * degree * 3));
    //[1][0][1] [1][1][1] [1][2][1] ... [1][_degree-1][1]
    for(unsigned int i = 0; i < degree; i+=3) {
        unused[i] = 1;
        unused[i + 1] = i/3;
        unused[i + 2] = 1;
    }
}


inline unsigned int TrailStructure::getNextUnused() {
    if(flags.at(1)) return 0; //black node
    if(!flags.at(0)) flags.at(0).flip(); //set to grey
    unsigned int prevLink = unused[nextUnused-1];
    unsigned int nextLink = unused[nextUnused+1];

    unsigned int temp;

    if(prevLink*3 > nextUnused) { //circle around
        temp = (degree * 3) - (prevLink * 3) + nextUnused;
    } else {
        temp = nextUnused - prevLink * 3;
    }
    if(temp == nextUnused) { //no other element, this is last
        unsigned int retVal = nextUnused;
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
    unsigned int retVal = nextUnused;
    nextUnused = temp;
    flags.at(2).flip(); //taking an arc flips the parity

    return retVal;
}


unsigned int TrailStructure::getMatched(unsigned int start, unsigned int idx) {
    
    //check if the idx is present in the married structure
    if(married[0] == idx) return married[1];
    if(married[1] == idx) return married[0];
    if(married[2] == idx) return married[3];
    if(married[3] == idx) return married[2];
    
    if(!matched[idx]) return idx; //has no match

    //get start idx for the dyck word
    unsigned int j = start;
    unsigned int p = 0;
    unsigned int stack[degree/2];
    do {
        if(matched[j]) { //only push matched index
            if(inAndOut[j]) { // '('
                stack[p++] = j;
            } else {
                unsigned int i = stack[--p];
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

unsigned int TrailStructure::leave() {
    unsigned int u = getNextUnused();
    return u == 0 ? (unsigned int) - 1 : unused[u];
}

unsigned int TrailStructure::enter(unsigned int i) {
    i = i*3+1; //multiple index so it works with the actual array.

    if(flags.at(1)) return (unsigned int) - 1;
    if(!flags.at(0)) flags.at(0).flip(); //set to grey

    unsigned int prevLink = unused[i-1];
    unsigned int nextLink = unused[i+1];

    unsigned int temp;

    if(prevLink*3 > i) { //circle around
        temp = (degree * 3) - (prevLink * 3) + i;
    } else {
        temp = i - prevLink * 3;
    }
    if(temp == i) { //no other element, this is last
        flags.at(1).flip(); //should be 0 now
        return (unsigned int) - 1; //returns non-value
    }
    unused[temp+1] += nextLink;

    if(nextLink*3 + i > degree) { //circle around
        temp = (nextLink * 3) - (degree * 3)   + i;
    } else {
        temp = i - nextLink * 3;
    }
    unused[temp-1] += prevLink;

    //not needed, we flip twice since we take another edge out now
    //flags.at(2).flip(); //taking an arc flips the parity

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

bool TrailStructure::isEven() {
    return flags.at(2);
}

void TrailStructure::marry(unsigned int i, unsigned int o) {
    unsigned int iMatch = getMatched(i);
}


