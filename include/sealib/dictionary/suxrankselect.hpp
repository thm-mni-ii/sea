#ifndef SEALIB_DICTIONARY_SUXRANKSELECT_HPP_
#define SEALIB_DICTIONARY_SUXRANKSELECT_HPP_
#include <sealib/collection/bitset.h>

#include "../../../extern/sux/sux/bits/Rank9Sel.hpp"

namespace Sealib {
class SuxRankSelect {
 private:
    Sealib::Bitset<uint64_t> _bs;
    sux::bits::Rank9Sel<> _rs;
    size_t _max_rank;

 public:
    SuxRankSelect(const Bitset<uint64_t>& bs)
        : _bs(bs), _rs(_bs.data(), _bs.size()), _max_rank{_rs.rank(_bs.size())} {}

    SuxRankSelect(Bitset<uint64_t>&& bs)
        : _bs(bs), _rs(_bs.data(), _bs.size()), _max_rank{_rs.rank(_bs.size())} {}

    SuxRankSelect(SuxRankSelect&& rs)
        : _bs(rs._bs), _rs(rs._bs.data(), rs._bs.size()), _max_rank{_rs.rank(_bs.size())} {}

    size_t rank(size_t idx) {
        if(idx > _bs.size() || idx == 0) {
            return INVALID;
        }
        return _rs.rank(idx-1) + (_bs[idx-1] ? 1 : 0);
    }

    size_t select(size_t idx) {
        if(idx > _max_rank || idx == 0) {
            return INVALID;
        }
        return _rs.select(idx-1) + 1;
    }

    size_t size() { return _bs.size(); }

    size_t maxRank() { return _max_rank; }

    const Sealib::Bitset<uint64_t>& getBitset() const {
        return _bs;
    }
};
}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_SUXRANKSELECT_HPP_
