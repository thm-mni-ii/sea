//
// Created by jmeintrup on 16.05.18.
//

#ifndef SEA_DIFFERENCELIST_H
#define SEA_DIFFERENCELIST_H

#include <sealib/dynamicbitsetextension.h>
#include <iterator>

namespace Sealib {

class DifferenceList {
private:
 boost::dynamic_bitset<unsigned char> bits;
 unsigned long currValue;
 unsigned long iterationValue;
 unsigned long iterationIdx;
 unsigned long idx;
 unsigned char smallVal;
 unsigned char smallBits;
 unsigned char largeBits;


 bool reverse;

public:
 DifferenceList(unsigned long maxElements_, unsigned long maxValue_);
 ~DifferenceList();

 void reset();
 unsigned long next();
 void setReverse();
 void setForward();
 void add(unsigned long val);

 void removeLast();
};
}  // namespace Sealib
#endif //SEA_DIFFERENCELIST_H
