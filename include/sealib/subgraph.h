#ifndef SEALIB_SUBGRAPH_H_
#define SEALIB_SUBGRAPH_H_
#include <sealib/bitset.h>
#include <sealib/rankselect.h>
#include <tuple>

namespace Sealib {
class SubGraph {
 public:
    typedef Sealib::Bitset<unsigned char> bitset_t;
    typedef Sealib::RankSelect rankselect_t;

 protected:
    rankselect_t qSelect;
    rankselect_t pSelect;
    SubGraph(const bitset_t &q, const bitset_t &p) : qSelect(q), pSelect(p) {}

 public:
    virtual ~SubGraph() = default;
    SubGraph() = delete;

    virtual unsigned long degree(unsigned long v) const = 0;

    virtual unsigned long head(unsigned long u, unsigned long k) const = 0;

    virtual std::tuple<unsigned long, unsigned long>
    mate(unsigned long u, unsigned long k) const = 0;

    virtual unsigned long order() const = 0;

    virtual unsigned long arcNumber(unsigned long j, unsigned long k) const = 0;

    virtual std::tuple<unsigned long, unsigned long> inverseArcNumber(unsigned long r) const = 0;

    virtual unsigned long translateVertex(unsigned long u) const = 0;

    virtual unsigned long translateArc(unsigned long e) const = 0;
};
}  // namespace Sealib
#endif  // SEALIB_SUBGRAPH_H_
