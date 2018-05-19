//
// Created by jmeintrup on 12.05.18.
//

#ifndef SEA_DYCKMATCHINGSTRUCTURE_H
#define SEA_DYCKMATCHINGSTRUCTURE_H

#include <include/sealib/dynamicbitsetextension.h>
namespace Sealib {
    class DyckMatchingStructure {
    public:
        static void printDyckWord(const boost::dynamic_bitset<>& word);
        static void printDyckWord(const boost::dynamic_bitset<>& word, unsigned long breaks);
        unsigned long findMatchNaive(unsigned long idx);

        virtual unsigned long findMatch(unsigned long idx);

        explicit DyckMatchingStructure(const boost::dynamic_bitset<> &word_);

        const boost::dynamic_bitset<> &getWord() const;

    protected:
        const boost::dynamic_bitset<> word;
    };
}  // namespace Sealib
#endif //SEA_DYCKMATCHINGSTRUCTURE_H
