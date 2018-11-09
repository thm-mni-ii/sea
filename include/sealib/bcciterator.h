#ifndef SEALIB_BCCITERATOR_H_
#define SEALIB_BCCITERATOR_H_
#include "sealib/basicgraph.h"
#include "sealib/edgemarker.h"
#include "sealib/iterator.h"

namespace Sealib {
class BCCIterator : Iterator<uint> {
 public:
    explicit BCCIterator(BasicGraph *g);
    BCCIterator(BasicGraph *g, EdgeMarker *e);

    void init() override;

    void setOrigin(uint u, uint v);

    bool more() override;

    uint next() override;

 private:
    BasicGraph *g;
    EdgeMarker e;
};
}  // namespace Sealib

#endif  // SEALIB_BCCITERATOR_H_
