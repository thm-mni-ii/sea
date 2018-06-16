#include "sealib/compactarray.h"
#include <stdio.h>

void CompactArray::insert(unsigned int i, unsigned int p) {
  // values per group: 3/e, groups per datum: dataWidth/groupWidth => values per
  // datum: dataWidth/(groupWidth/(3/e))
  if (i >= valueCount) return;
  int groupIndex = static_cast<int>(floor(i / static_cast<double>(3 / e)));
  int dataIndex = static_cast<int>(
      floor(groupIndex / (dataWidth / static_cast<double>(groupWidth))));
  int groupOffset = static_cast<int>(
      fmod(groupIndex, dataWidth / static_cast<double>(groupWidth)));
  int valueOffset = static_cast<int>(fmod(i, 3 / e));
  // insert p into slot 'data X + group Y + value Z'
  unsigned a = data[dataIndex];
  unsigned b1 = (unsigned)pow(2, valueWidth) - 1,
           b2 = (dataWidth - valueWidth) - groupOffset * groupWidth -
                valueOffset * valueWidth;
  unsigned b = b1 << b2;
  unsigned c = a & ~b;
  unsigned d = c | (p << b2);
  data[dataIndex] = d;
}

unsigned int CompactArray::get(unsigned int selector, unsigned int i) {
  int groupIndex = static_cast<int>(floor(i / (3 / e)));
  int dataIndex = static_cast<int>(
      floor(groupIndex / (dataWidth / static_cast<double>(groupWidth))));
  int groupOffset = static_cast<int>(
      fmod(groupIndex, dataWidth / static_cast<double>(groupWidth)));
  int valueOffset = static_cast<int>(fmod(i, 3 / e));
  switch (selector) {
    case COMPACTARRAY_DATA:
      if (i > dataCount)
        return COMPACTARRAY_FAULT;
      else
        return data[i];
    case COMPACTARRAY_GROUP: {
      // get result from data X + group Y
      unsigned a = data[dataIndex];
      unsigned b1 = (unsigned)pow(2, groupWidth) - 1,
               b2 = (dataWidth - groupWidth) - (groupOffset + 1) * groupWidth;
      unsigned b = b1 << b2;
      unsigned c = a & b;
      int d = c >> b2;
      return d;
    }
    case COMPACTARRAY_VALUE: {
      unsigned a = data[dataIndex];
      unsigned b1 = (unsigned)pow(2, valueWidth) - 1,
               b2 = (dataWidth - valueWidth) - groupOffset * groupWidth -
                    valueOffset * valueWidth;
      unsigned b = b1 << b2;
      unsigned c = a & b;
      int d = c >> b2;
      return d;
    }
    default:
      return COMPACTARRAY_FAULT;
  }
}
unsigned int CompactArray::get(unsigned int i) {
  return get(COMPACTARRAY_VALUE, i);
}

CompactArray::CompactArray(unsigned int count, double epsilon) {
  e = epsilon;
  // the following is valid if the inserted values can have 3 states:
  valueCount = count;
  valueWidth = static_cast<int>(ceil(log(3) / log(2)));
  groupWidth = static_cast<int>(
      ceil(3 * (log(3) / log(2)) /
           e));  // bits for a group of 3/e (e.g. 2) consec. colors
  groupCount = static_cast<int>(
      ceil(count / (groupWidth / static_cast<double>(valueWidth))));
  dataWidth = 8 * sizeof(unsigned int);
  dataCount = static_cast<int>(
      ceil((groupCount * groupWidth) / static_cast<double>(dataWidth)));
  data = new unsigned int[dataCount];
}

CompactArray::~CompactArray() { delete[] data; }
