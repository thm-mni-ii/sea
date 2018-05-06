//
// Created by jmeintrup on 03.05.18.
//

#ifndef SEA_RANKSELECT_H
#define SEA_RANKSELECT_H

#include <boost/dynamic_bitset.hpp>

namespace Sealib{
    class RankSelect {
        boost::dynamic_bitset<> bitset;

    public:
        unsigned long select(unsigned int k) const;
        unsigned long rank(unsigned int k) const;

        explicit RankSelect(const boost::dynamic_bitset<> &bitset);

        RankSelect();
    };
}
#endif //SEA_RANKSELECT_H
