#include <sealib/dyckwordlexicon.h>
#include <iostream>

Sealib::DyckWordLexicon::DyckWordLexicon(unsigned int wordLength_) : wordLength(wordLength_) {
    if (wordLength < 2) {
        wordLength = 2;
    } else if (wordLength % 2 != 0) {
        wordLength--;
    }

    Sealib::Bitset<unsigned char> baseWord(wordLength);
    baseWord[0] = 1;  // "("
    generateWords(baseWord, 1, 1, 0);
}

void Sealib::DyckWordLexicon::generateWords(Sealib::Bitset<unsigned char> word,
                                            unsigned int i,
                                            unsigned int mOpen,
                                            unsigned int mClosed) {
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

const std::vector<Sealib::Bitset<unsigned char>> &Sealib::DyckWordLexicon::getLexicon() {
    return lexicon;
}
unsigned int Sealib::DyckWordLexicon::getWordLength() {
    return wordLength;
}
