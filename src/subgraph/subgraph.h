#ifndef SRC_SUBGRAPH_SUBGRAPH_H_
#define SRC_SUBGRAPH_SUBGRAPH_H_
#include <sealib/collection/bitset.h>
#include <sealib/dictionary/rankselect.h>
#include <sealib/subgraphstack.h>
#include <string>
#include <tuple>

namespace Sealib {
class SubGraph {
    friend class BaseSubGraph;
    friend class RecursiveSubGraph;
 public:
    typedef Sealib::Bitset<uint8_t> bitset_t;
    typedef Sealib::RankSelect rankselect_t;
    typedef Sealib::SubGraphStack stack_t;

 protected:
    uint64_t sidx;
    uint64_t ridx;
    stack_t *stack;
    rankselect_t *qSelect;
    rankselect_t *pSelect;

    SubGraph(uint64_t sidx_, uint64_t ridx_, stack_t *stack_)
        : sidx(sidx_),
          ridx(ridx_),
          stack(stack_),
          qSelect(nullptr),
          pSelect(nullptr) {}

    inline uint64_t select_q(uint64_t i) const {
        return qSelect->select(i);
    }

    inline uint64_t select_p(uint64_t i) const {
        return pSelect->select(i);
    }

    inline uint64_t rank_q(uint64_t i) const {
        return qSelect->rank(i);
    }

    inline uint64_t rank_p(uint64_t i) const {
        return pSelect->rank(i);
    }

 public:
    inline uint64_t getSidx() const {
        return sidx;
    }

    inline uint64_t getRidx() const {
        return ridx;
    }

    uint64_t degree(uint64_t u) const {
        if (u == 0) {
            throw std::invalid_argument("u needs to be > 0");
        }
        uint64_t a = select_p(rank_q(u));  //  pSelect.select(qSelect.rank(v));
        uint64_t b = select_p(rank_q(u - 1));  // pSelect.select(qSelect.rank(v - 1));

        if (a == b) {
            return 0;
        } else if (b == (uint64_t) -1) {
            return a;
        } else {
            return a - b;
        }
    }

    virtual uint64_t head(uint64_t u, uint64_t k) const = 0;

    inline uint64_t head(std::tuple<uint64_t, uint64_t> uk) const {
        return head(std::get<0>(uk), std::get<1>(uk));
    }

    virtual std::tuple<uint64_t, uint64_t>
    mate(uint64_t u, uint64_t k) const = 0;

    inline std::tuple<uint64_t, uint64_t>
    mate(std::tuple<uint64_t, uint64_t> uk) const {
        return mate(std::get<0>(uk), std::get<1>(uk));
    }

    inline uint64_t order() const {
        return qSelect->size();
    }

    uint64_t g(uint64_t j, uint64_t k) const {
        if (j == 0 || k == 0) {
            throw std::invalid_argument(
                "j and k need to be > 0! (j,k)=(" +
                    std::to_string(j) + "," + std::to_string(k) + ")");
        }

        uint64_t deg = degree(j);
        if (deg == 0 || k > deg) {
            throw std::out_of_range("node j has a degree < k! (j,k)=(" +
                std::to_string(j) + "," + std::to_string(k) + ")");
        }

        uint64_t qRank = rank_q(j);  // qSelect.rank(j);
        uint64_t n = 0;
        if (qRank > 1) {
            n = select_p(qRank - 1);  // pSelect.select(qRank - 1) - 1;
        }
        uint64_t arc = n + k;
        return arc;  // pSelect.rank(n + 1) != pSelect.rank(arc)
    }

    inline uint64_t gMax() {
        return pSelect->size();
    }

    inline uint64_t g(std::tuple<uint64_t, uint64_t> jk) const {
        return g(std::get<0>(jk), std::get<1>(jk));
    }

    std::tuple<uint64_t, uint64_t> gInv(uint64_t r) const {
        if (r == 0) {
            throw std::invalid_argument("r needs to be > 0 (r = "
                                            + std::to_string(r) + ")");
        }
        uint64_t j = r == 1 ? 0 : rank_p(r - 1);  // pSelect.rank(r - 1);
        if (j == (uint64_t) -1) {
            throw std::out_of_range("out of range - no arc r exists! (r = "
                                        + std::to_string(r) + ")");
        }
        j++;
        uint64_t a = select_q(j);  // qSelect.select(j);
        if (a == (uint64_t) -1) {
            throw std::out_of_range("out of range - no arc r exists! (r = "
                                        + std::to_string(r) + ")");
        }
        uint64_t b = select_p(j - 1);  // pSelect.select(j - 1);
        b = b == (uint64_t) -1 ? 0 : b;
        return std::tuple<uint64_t, uint64_t>(a, r - b);
    }

    virtual uint64_t phi(uint64_t u) const = 0;

    virtual uint64_t psi(uint64_t a) const = 0;

    virtual uint64_t phiInv(uint64_t u) const = 0;

    virtual uint64_t psiInv(uint64_t a) const = 0;
    virtual ~SubGraph() {
        delete pSelect;
        delete qSelect;
    }
    SubGraph() = delete;
};
}  // namespace Sealib
#endif  // SRC_SUBGRAPH_SUBGRAPH_H_
