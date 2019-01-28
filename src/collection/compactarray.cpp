#include "sealib/collection/compactarray.h"
#include <math.h>
#include <stdexcept>
#include "sealib/_types.h"

#define PRELUDE                                                              \
    uint g1 = i * valueWidth / BITSIZE, g2 = (i + 1) * valueWidth / BITSIZE; \
    uint b1 = (i * valueWidth) % BITSIZE,                                    \
         b2 = ((i + 1) * valueWidth) % BITSIZE - 1;                          \
    uint s = BITSIZE - b2 - 1;

#define IF_SINGLE_GROUP if (g1 == g2) {
#define ELSE_IF_DOUBLE_GROUP                          \
    }                                                 \
    else { /*NOLINT*/                                 \
        uint startMask = (ONE << (BITSIZE - b1)) - 1, \
             endMask = (ONE << (b2 + 1)) - 1;

#define END }

namespace Sealib {

static const size_t BITSIZE = 8 * sizeof(uint);
static const uint ONE = 1;

void CompactArray::insert(uint i, uint v) {
    PRELUDE
    IF_SINGLE_GROUP
    data[g1] &= ~(singleMask << s);
    data[g1] |= v << s;
    ELSE_IF_DOUBLE_GROUP
    data[g1] &= ~startMask;
    data[g1] |= v >> (b2 + 1);
    data[g2] &= ~(endMask << s);
    data[g2] |= (v & endMask) << s;
    END
}

uint CompactArray::get(uint i) const {
    uint r;
    PRELUDE
    IF_SINGLE_GROUP
    r = (data[g1] >> s) & singleMask;
    ELSE_IF_DOUBLE_GROUP
    r = (data[g1] & startMask) << (b2 + 1);
    r |= (data[g2] & (endMask << s)) >> s;
    END return r;
}

CompactArray::CompactArray(uint size, uint values)
    : valueWidth(static_cast<uint>(ceil(log2(values)))),
      singleMask((ONE << valueWidth) - 1),
      data(size * valueWidth / BITSIZE + 1) {
    if (valueWidth >= sizeof(uint) * 8) {
        throw std::domain_error("v is too big (max v = bitsize(uint))");
    }
}

}  // namespace Sealib
