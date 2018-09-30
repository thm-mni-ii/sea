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

    inline unsigned long select_q(unsigned long i) const {
        return qSelect.select(i);
    }

    inline unsigned long select_p(unsigned long i) const {
        return pSelect.select(i);
    }

    inline unsigned long rank_q(unsigned long i) const {
        return qSelect.rank(i);
    }

    inline unsigned long rank_p(unsigned long i) const {
        return pSelect.rank(i);
    }

 public:
    virtual ~SubGraph() = default;
    SubGraph() = delete;

    virtual unsigned long degree(unsigned long u) const = 0;

    virtual unsigned long head(unsigned long u, unsigned long k) const = 0;

    inline unsigned long head(std::tuple<unsigned long, unsigned long> uk) const {
        return head(std::get<0>(uk), std::get<0>(uk));
    }

    virtual std::tuple<unsigned long, unsigned long>
    mate(unsigned long u, unsigned long k) const = 0;

    inline std::tuple<unsigned long, unsigned long>
    mate(std::tuple<unsigned long, unsigned long> uk) const {
        return mate(std::get<0>(uk), std::get<0>(uk));
    }

    inline unsigned long order() const {
        return qSelect.size();
    };

    virtual unsigned long g(unsigned long j, unsigned long k) const {
        if (j == 0 || k == 0) {
            throw std::invalid_argument(
                "j and k need to be > 0! (j,k)=(" +
                    std::to_string(j) + "," + std::to_string(k) + ")");
        }
        if (degree(j) == 0) {
            throw std::out_of_range("isolated node! (j = " + std::to_string(j) + ")");
        }

        unsigned long qRank = rank_q(j);  // qSelect.rank(j);
        unsigned long n = 0;
        if (qRank > 1) {
            n = select_p(qRank - 1) - 1;  // pSelect.select(qRank - 1) - 1;
        }
        unsigned long arc = n + k;

        if (rank_p(n + 1) != rank_p(arc)) {  // pSelect.rank(n + 1) != pSelect.rank(arc)
            throw std::out_of_range(
                "node j has less than k arcs! (j,k)=("
                    + std::to_string(j) + "," + std::to_string(k) + ")");
        }
        return arc;
    }

    inline unsigned long g(std::tuple<unsigned long, unsigned long> jk) const {
        return g(std::get<0>(jk), std::get<0>(jk));
    }

    virtual std::tuple<unsigned long, unsigned long> gInv(unsigned long r) const {
        if (r == 0) {
            throw std::invalid_argument("r needs to be > 0 (r = " + std::to_string(r) + ")");
        }
        unsigned long j = rank_p(r);  // pSelect.rank(r);
        if (j == (unsigned long) -1) {
            throw std::out_of_range("out of range - no arc r exists! (r = " + std::to_string(r) + ")");
        }
        j++;
        unsigned long a = select_q(j);  // qSelect.select(j);
        if (a == (unsigned long) -1) {
            throw std::out_of_range("out of range - no arc r exists! (r = " + std::to_string(r) + ")");
        }
        unsigned long b = select_p(j - 1);  // pSelect.select(j - 1);
        b = b == (unsigned long) -1 ? 0 : b - 1;
        return {a, r - b};
    }

    virtual unsigned long phi(unsigned long u) const = 0;

    virtual unsigned long psi(unsigned long a) const = 0;

    virtual unsigned long phiInv(unsigned long u) const = 0;

    virtual unsigned long psiInv(unsigned long a) const = 0;
};
}  // namespace Sealib
#endif  // SEALIB_SUBGRAPH_H_
