#ifndef SEALIB_DYCKWORDLEXICON_H_
#define SEALIB_DYCKWORDLEXICON_H_

#include <sealib/collection/bitset.h>
#include <vector>
#include <ostream>

namespace Sealib {
/**
 * Lexicon of all DyckWords of a specified length.
 * Implementation based on the article:
 * "Generating and ranking of Dyck words" by Zoltan Kasa,
 * Acta Universitatis Sapientiae, Informatica, 1, 1 (2009) 109-118
 *
 * @author: Johannes Meintrup
 */
class DyckWordLexicon {
 private:
    std::vector<Sealib::Bitset<uint8_t>> lexicon;

    uint32_t wordLength;
    void generateWords(Sealib::Bitset<uint8_t> word,
                       uint32_t i,
                       uint32_t mOpen,
                       uint32_t mClosed);

 public:
    /**
     * @param wordLength_ Length of the dyckwords to be created,
     * the object will hold all words of the specified length.
     * wordLength_ has to be even, or it will be decremented by one.
     * If wordLength_ is smaller than two, it will be set to two.
     */
    explicit DyckWordLexicon(uint32_t wordLength_);

    /**
     * @return const ref to the lexicon
     */
    const std::vector<Sealib::Bitset<uint8_t>> &getLexicon();

    /**
     * @return length of the generated words
     */
    uint32_t getWordLength();
};
}  // namespace Sealib
#endif  // SEALIB_DYCKWORDLEXICON_H_
