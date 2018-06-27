#include "sealib/compactarray.h"
#include <stdio.h>

void CompactArray::insert(unsigned int i, unsigned int p) {
  // values per group: 3/e, groups per datum: dataWidth/groupWidth => values per
  // datum: dataWidth/(groupWidth/(3/e))
  if (i >= valueCount) return;
  double groupIndex = floor(i / valuesPerGroup);
  unsigned dataIndex =
      static_cast<unsigned>(floor(groupIndex / groupsPerDatum));
  double groupOffset = fmod(groupIndex, groupsPerDatum);
  double valueOffset = fmod(i, valuesPerGroup);
  // insert p into slot 'data X + group Y + value Z'
  unsigned a = data[dataIndex];
  unsigned b2 = static_cast<unsigned>((dataWidth - valueWidth) -
                                      groupOffset * groupWidth -
                                      valueOffset * valueWidth);
  unsigned b = shv1 << b2;
  unsigned c = a & ~b;
  unsigned d = c | (p << b2);
  data[dataIndex] = d;
}

unsigned int CompactArray::get(unsigned int selector, unsigned int i) {
  double groupIndex = floor(i / valuesPerGroup);
  unsigned dataIndex =
      static_cast<unsigned>(floor(groupIndex / groupsPerDatum));
  double groupOffset = fmod(groupIndex, groupsPerDatum);
  double valueOffset = fmod(i, valuesPerGroup);
  switch (selector) {
    case COMPACTARRAY_DATA:
      if (i > dataCount)
        return COMPACTARRAY_FAULT;
      else
        return data[i];
    case COMPACTARRAY_GROUP: {
      // get result from data X + group Y
      unsigned a = data[dataIndex];
      unsigned b2 = static_cast<unsigned>((dataWidth - groupWidth) -
                                          (groupOffset + 1) * groupWidth);
      unsigned b = shg1 << b2;
      unsigned c = a & b;
      int d = c >> b2;
      return d;
    }
    case COMPACTARRAY_VALUE: {
      unsigned a = data[dataIndex];
      unsigned b2 = static_cast<unsigned>((dataWidth - valueWidth) -
                                          groupOffset * groupWidth -
                                          valueOffset * valueWidth);
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
  valueCount = static_cast<double>(count);
  valueWidth = ceil(log(3) / log(2));
  groupWidth = ceil(3/e) * valueWidth;  // bits for a group of 3/e (e.g. 2) consec. colors
  groupCount = ceil(count / (groupWidth / valueWidth));
  dataWidth = 8 * sizeof(unsigned int);
  dataCount = ceil((groupCount * groupWidth) / dataWidth);
  data = new unsigned int[static_cast<unsigned>(dataCount)];
  valuesPerGroup = 3 / e;
  groupsPerDatum = dataWidth / groupWidth;
  shv1 = static_cast<unsigned>(pow(2, valueWidth) - 1);
  shg1 = static_cast<unsigned>(pow(2, groupWidth) - 1);
}

CompactArray::~CompactArray() { delete[] data; }
