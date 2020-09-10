#ifndef SRC_SUBGRAPH_SUBGRAPH_H_
#define SRC_SUBGRAPH_SUBGRAPH_H_
#include <sealib/collection/bitset.h>
#include <sealib/dictionary/rankselect.h>
#include <sealib/collection/subgraphstack.h>
#include <stdexcept>
#include <string>
#include <tuple>

namespace Sealib {

class ZeroArgumentGiven : public std::exception {
    const char* what() const noexcept {
        return "SubGraph: the argument(s) to this function must be > 0";
    }
};
class DegreeTooSmall : public std::exception {
    const char* what() const noexcept {
        return "SubGraph: function g(j,k): node j has a degree < k";
    }
};
class NoArcExists : public std::exception {
    const char* what() const noexcept {
        return "SubGraph: function gInv(r): no arc r exists";
    }
};

/**
 * Subgraph parent class.
 * @author Johannes Meintrup
 */
class SubGraph {
    friend class BaseSubGraph;
    friend class RecursiveSubGraph;
 public:
    typedef Sealib::Bitset<uint64_t> bitset_t;
    typedef Sealib::Rank9Select rankselect_t;
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

    SubGraph(SubGraph const &) = default;
    SubGraph & operator=(SubGraph const &) = default;
    SubGraph(SubGraph &&) = default;
    SubGraph & operator=(SubGraph &&) = default;

    inline uint64_t select_q(uint64_t i) {
        return qSelect->select(i);
    }

    inline uint64_t select_p(uint64_t i) {
        return pSelect->select(i);
    }

    inline uint64_t rank_q(uint64_t i) {
        return qSelect->rank(i);
    }

    inline uint64_t rank_p(uint64_t i) {
        return pSelect->rank(i);
    }

 public:
    virtual ~SubGraph() {
        delete pSelect;
        delete qSelect;
    }

    inline uint64_t getSidx() {
        return sidx;
    }

    inline uint64_t getRidx() {
        return ridx;
    }

    uint64_t degree(uint64_t u) {
        if (u == 0) {
            throw ZeroArgumentGiven();
        }
        uint64_t a = select_p(rank_q(u));  //  pSelect.select(qSelect.rank(v));
        uint64_t b = select_p(rank_q(u - 1));  // pSelect.select(qSelect.rank(v - 1));

        if (a == b) {
            return 0;
        } else if (b == INVALID) {
            return a;
        } else {
            return a - b;
        }
    }

    virtual uint64_t head(uint64_t u, uint64_t k) = 0;

    inline uint64_t head(std::tuple<uint64_t, uint64_t> uk) {
        return head(std::get<0>(uk), std::get<1>(uk));
    }

    virtual std::tuple<uint64_t, uint64_t>
    mate(uint64_t u, uint64_t k) = 0;

    inline std::tuple<uint64_t, uint64_t>
    mate(std::tuple<uint64_t, uint64_t> uk) {
        return mate(std::get<0>(uk), std::get<1>(uk));
    }

    inline uint64_t order() {
        return qSelect->size();
    }

    uint64_t g(uint64_t j, uint64_t k) {
        if (j == 0 || k == 0) {
            throw ZeroArgumentGiven();
        }

        uint64_t deg = degree(j);
        if (deg == 0 || k > deg) {
            throw DegreeTooSmall();
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

    inline uint64_t g(std::tuple<uint64_t, uint64_t> jk) {
        return g(std::get<0>(jk), std::get<1>(jk));
    }

    std::tuple<uint64_t, uint64_t> gInv(uint64_t r) {
        if (r == 0) {
            throw ZeroArgumentGiven();
        }
        uint64_t j = r == 1 ? 0 : rank_p(r - 1);  // pSelect.rank(r - 1);
        if (j == INVALID) {
            throw NoArcExists();
        }
        j++;
        uint64_t a = select_q(j);  // qSelect.select(j);
        if (a == INVALID) {
            throw NoArcExists();
        }
        uint64_t b = select_p(j - 1);  // pSelect.select(j - 1);
        b = b == INVALID ? 0 : b;
        return std::tuple<uint64_t, uint64_t>(a, r - b);
    }

    virtual uint64_t phi(uint64_t u) = 0;

    virtual uint64_t psi(uint64_t a) = 0;

    virtual uint64_t phiInv(uint64_t u) = 0;

    virtual uint64_t psiInv(uint64_t a) = 0;
};
}  // namespace Sealib
#endif  // SRC_SUBGRAPH_SUBGRAPH_H_
