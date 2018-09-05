#ifndef SEALIB_DYCKWORDLEXICON_H_
#define SEALIB_DYCKWORDLEXICON_H_

#include <sealib/bitset.h>
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
    std::vector<Sealib::Bitset<unsigned char>> lexicon;

    unsigned int wordLength;
    void generateWords(Sealib::Bitset<unsigned char> word, int i, int mOpen, int mClosed);

 public:
    /**
     * @param wordLength_ Length of the dyckwords to be created, the object will hold all words of the specified length.
     * wordLength_ has to be even, or it will be decremented by one.
     * If wordLength_ is smaller than two, it will be set to two.
     */
    explicit DyckWordLexicon(unsigned int wordLength_);

    /**
     * @return const ref to the lexicon
     */
    const std::vector<Sealib::Bitset<unsigned char>> &getLexicon();

    /**
     * @return length of the generated words
     */
    unsigned int getWordLength();
};
}  // namespace Sealib
#endif  // SEALIB_DYCKWORDLEXICON_H_
