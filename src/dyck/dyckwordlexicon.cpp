#include <sealib/dyckwordlexicon.h>
#include <iostream>

Sealib::DyckWordLexicon::DyckWordLexicon(uint32_t wordLength_) : wordLength(wordLength_) {
    if (wordLength < 2) {
        wordLength = 2;
    } else if (wordLength % 2 != 0) {
        wordLength--;
    }

    Sealib::Bitset<uint8_t> baseWord(wordLength);
    baseWord[0] = 1;  // "("
    generateWords(baseWord, 1, 1, 0);
}

void Sealib::DyckWordLexicon::generateWords(Sealib::Bitset<uint8_t> word,
                                            uint32_t i,
                                            uint32_t mOpen,
                                            uint32_t mClosed) {
    if (mOpen < wordLength / 2
        && mClosed < wordLength / 2
        && mOpen > mClosed) {  // can add "(" or ")"
        word[i] = 1;
        generateWords(word, i + 1, mOpen + 1, mClosed);

        word[i] = 0;
        generateWords(word, i + 1, mOpen, mClosed + 1);
    } else if ((mOpen < wordLength / 2
        && mClosed < wordLength / 2
        && mOpen == mClosed)  // can add "("
        || (mOpen < wordLength / 2 && mClosed == wordLength / 2)) {
        word[i] = 1;
        generateWords(word, i + 1, mOpen + 1, mClosed);
    } else if (mOpen == wordLength / 2
        && mClosed < wordLength / 2) {  // can add ")"
        word[i] = 0;
        generateWords(word, i + 1, mOpen, mClosed + 1);
    } else if (mOpen == mClosed
        && (mOpen + mClosed) == wordLength) {  // we have a dyckword!
        lexicon.push_back(word);
    }
}

const std::vector<Sealib::Bitset<uint8_t>> &Sealib::DyckWordLexicon::getLexicon() {
    return lexicon;
}
uint32_t Sealib::DyckWordLexicon::getWordLength() {
    return wordLength;
}
