#include "sealib/compactarray.h"
#include <stdio.h>

void CompactArray::insert(unsigned int i, unsigned int p) {
  // values per group: 3/e, groups per datum: dataWidth/groupWidth => values per
  // datum: dataWidth/(groupWidth/(3/e))
  if (i >= valueCount) return;
  int groupIndex = static_cast<int>(floor(i / valuesPerGroup));
  int dataIndex = static_cast<int>(floor(groupIndex / groupsPerDatum));
  int groupOffset = static_cast<int>(fmod(groupIndex, groupsPerDatum));
  int valueOffset = static_cast<int>(fmod(i, valuesPerGroup));
  // insert p into slot 'data X + group Y + value Z'
  unsigned a = data[dataIndex];
  unsigned b2 = (dataWidth - valueWidth) - groupOffset * groupWidth -
                valueOffset * valueWidth;
  unsigned b = shv1 << b2;
  unsigned c = a & ~b;
  unsigned d = c | (p << b2);
  data[dataIndex] = d;
}

unsigned int CompactArray::get(unsigned int selector, unsigned int i) {
  int groupIndex = static_cast<int>(floor(i / valuesPerGroup));
  int dataIndex = static_cast<int>(floor(groupIndex / groupsPerDatum));
  int groupOffset = static_cast<int>(fmod(groupIndex, groupsPerDatum));
  int valueOffset = static_cast<int>(fmod(i, valuesPerGroup));
  switch (selector) {
    case COMPACTARRAY_DATA:
      if (i > dataCount)
        return COMPACTARRAY_FAULT;
      else
        return data[i];
    case COMPACTARRAY_GROUP: {
      // get result from data X + group Y
      unsigned a = data[dataIndex];
      unsigned b2 = (dataWidth - groupWidth) - (groupOffset + 1) * groupWidth;
      unsigned b = shg1 << b2;
      unsigned c = a & b;
      int d = c >> b2;
      return d;
    }
    case COMPACTARRAY_VALUE: {
      unsigned a = data[dataIndex];
      unsigned b2 = (dataWidth - valueWidth) - groupOffset * groupWidth -
                    valueOffset * valueWidth;
      unsigned b = shv1 << b2;
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
  valuesPerGroup = 3 / e;
  groupsPerDatum = dataWidth / static_cast<double>(groupWidth);
  shv1 = static_cast<unsigned>(pow(2, valueWidth) - 1);
  shg1 = static_cast<unsigned>(pow(2, groupWidth) - 1);
}

CompactArray::~CompactArray() { delete[] data; }
