#ifndef SRC_SUBGRAPH_H_
#define SRC_SUBGRAPH_H_
#include <sealib/bitset.h>
#include <sealib/rankselect.h>
#include <sealib/subgraphstack.h>
#include <string>
#include <tuple>

namespace Sealib {
class SubGraph {
    friend class BaseSubGraph;
    friend class RecursiveSubGraph;
 public:
    typedef Sealib::Bitset<unsigned char> bitset_t;
    typedef Sealib::RankSelect rankselect_t;
    typedef Sealib::SubGraphStack stack_t;

 protected:
    unsigned long sidx;
    unsigned long ridx;
    stack_t *stack;
    rankselect_t *qSelect;
    rankselect_t *pSelect;

    SubGraph(unsigned long sidx_, unsigned long ridx_, stack_t *stack_)
        : sidx(sidx_),
          ridx(ridx_),
          stack(stack_),
          qSelect(nullptr),
          pSelect(nullptr) {}

    inline unsigned long select_q(unsigned long i) const {
        return qSelect->select(i);
    }

    inline unsigned long select_p(unsigned long i) const {
        return pSelect->select(i);
    }

    inline unsigned long rank_q(unsigned long i) const {
        return qSelect->rank(i);
    }

    inline unsigned long rank_p(unsigned long i) const {
        return pSelect->rank(i);
    }

 public:
    inline unsigned long getSidx() const {
        return sidx;
    }

    inline unsigned long getRidx() const {
        return ridx;
    }

    unsigned long degree(unsigned long u) const {
        if (u == 0) {
            throw std::invalid_argument("u needs to be > 0");
        }
        unsigned long a = select_p(rank_q(u));  //  pSelect.select(qSelect.rank(v));
        unsigned long b = select_p(rank_q(u - 1));  // pSelect.select(qSelect.rank(v - 1));

        if (a == b) {
            return 0;
        } else if (b == (unsigned long) -1) {
            return a;
        } else {
            return a - b;
        }
    }

    virtual unsigned long head(unsigned long u, unsigned long k) const = 0;

    inline unsigned long head(std::tuple<unsigned long, unsigned long> uk) const {
        return head(std::get<0>(uk), std::get<1>(uk));
    }

    virtual std::tuple<unsigned long, unsigned long>
    mate(unsigned long u, unsigned long k) const = 0;

    inline std::tuple<unsigned long, unsigned long>
    mate(std::tuple<unsigned long, unsigned long> uk) const {
        return mate(std::get<0>(uk), std::get<1>(uk));
    }

    inline unsigned long order() const {
        return qSelect->size();
    }

    unsigned long g(unsigned long j, unsigned long k) const {
        if (j == 0 || k == 0) {
            throw std::invalid_argument(
                "j and k need to be > 0! (j,k)=(" +
                    std::to_string(j) + "," + std::to_string(k) + ")");
        }

        unsigned long deg = degree(j);
        if (deg == 0 || k > deg) {
            throw std::out_of_range("node j has a degree < k! (j,k)=(" +
                std::to_string(j) + "," + std::to_string(k) + ")");
        }

        unsigned long qRank = rank_q(j);  // qSelect.rank(j);
        unsigned long n = 0;
        if (qRank > 1) {
            n = select_p(qRank - 1);  // pSelect.select(qRank - 1) - 1;
        }
        unsigned long arc = n + k;
        return arc;  // pSelect.rank(n + 1) != pSelect.rank(arc)
    }

    inline unsigned long gMax() {
        return pSelect->size();
    }

    inline unsigned long g(std::tuple<unsigned long, unsigned long> jk) const {
        return g(std::get<0>(jk), std::get<1>(jk));
    }

    std::tuple<unsigned long, unsigned long> gInv(unsigned long r) const {
        if (r == 0) {
            throw std::invalid_argument("r needs to be > 0 (r = "
                                            + std::to_string(r) + ")");
        }
        unsigned long j = r == 1 ? 0 : rank_p(r - 1);  // pSelect.rank(r - 1);
        if (j == (unsigned long) -1) {
            throw std::out_of_range("out of range - no arc r exists! (r = "
                                        + std::to_string(r) + ")");
        }
        j++;
        unsigned long a = select_q(j);  // qSelect.select(j);
        if (a == (unsigned long) -1) {
            throw std::out_of_range("out of range - no arc r exists! (r = "
                                        + std::to_string(r) + ")");
        }
        unsigned long b = select_p(j - 1);  // pSelect.select(j - 1);
        b = b == (unsigned long) -1 ? 0 : b;
        return {a, r - b};
    }

    virtual unsigned long phi(unsigned long u) const = 0;

    virtual unsigned long psi(unsigned long a) const = 0;

    virtual unsigned long phiInv(unsigned long u) const = 0;

    virtual unsigned long psiInv(unsigned long a) const = 0;
    virtual ~SubGraph() {
        delete pSelect;
        delete qSelect;
    }
    SubGraph() = delete;
};
}  // namespace Sealib
#endif  // SRC_SUBGRAPH_H_
