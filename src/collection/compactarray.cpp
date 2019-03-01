#include "sealib/collection/compactarray.h"
#include <math.h>
#include <stdexcept>
#include "sealib/_types.h"

#define PRELUDE                                           \
    uint64_t g1 = i * valueWidth / WORD_SIZE,             \
             g2 = (i + 1) * valueWidth / WORD_SIZE;       \
    uint64_t b1 = (i * valueWidth) % WORD_SIZE,           \
             b2 = ((i + 1) * valueWidth) % WORD_SIZE - 1; \
    uint64_t s = WORD_SIZE - b2 - 1;

#define IF_SINGLE_GROUP if (g1 == g2) {
#define ELSE_IF_DOUBLE_GROUP                                \
    }                                                       \
    else { /*NOLINT*/                                       \
        uint64_t startMask = (ONE << (WORD_SIZE - b1)) - 1, \
                 endMask = (ONE << (b2 + 1)) - 1;

#define END }

namespace Sealib {
static const uint64_t WORD_SIZE = 8 * sizeof(uint64_t);
static const uint64_t ONE = 1;

CompactArray::CompactArray(uint64_t size, uint64_t values)
    : valueWidth(static_cast<uint64_t>(ceil(log2(values)))),
      singleMask((ONE << valueWidth) - 1),
      data(size * valueWidth / WORD_SIZE + 1) {}

void CompactArray::insert(uint64_t i, uint64_t v) {
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

uint64_t CompactArray::get(uint64_t i) const {
    uint64_t r;
    PRELUDE
    IF_SINGLE_GROUP
    r = (data[g1] >> s) & singleMask;
    ELSE_IF_DOUBLE_GROUP
    r = (data[g1] & startMask) << (b2 + 1);
    r |= (data[g2] & (endMask << s)) >> s;
    END return r;
}

}  // namespace Sealib
