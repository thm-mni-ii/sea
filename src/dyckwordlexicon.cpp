#include <include/sealib/dyckwordlexicon.h>
#include <iostream>

Sealib::DyckWordLexicon::DyckWordLexicon(unsigned int wordLength_) : wordLength(wordLength_) {
  if (wordLength < 2) {
    wordLength = 2;
  } else if (wordLength % 2 != 0) {
    wordLength--;
  }

  generateWords(Sealib::Bitset<unsigned char>(wordLength), 1, 1, 0);
}

void Sealib::DyckWordLexicon::generateWords(Sealib::Bitset<unsigned char> x,
                                            int i,
                                            int n0,
                                            int n1) {
  if (n0 < wordLength / 2 && n1 < wordLength / 2 && n0 > n1) {  // can add "(" or ")"
    x[i] = 0;
    generateWords(x, i + 1, n0 + 1, n1);

    x[i] = 1;
    generateWords(x, i + 1, n0, n1 + 1);
  } else if ((n0 < wordLength / 2 && n1 < wordLength / 2 && n0 == n1)  // can add "("
      || (n0 < wordLength / 2 && n1 == wordLength / 2)) {
    x[i] = 0;
    generateWords(x, i + 1, n0 + 1, n1);
  } else if (n0 == wordLength / 2 && n1 < wordLength / 2) {  // can add ")"
    x[i] = 1;
    generateWords(x, i + 1, n0, n1 + 1);
  } else if (n0 == n1 && (n0 + n1) == wordLength) {  // we have a dyckword!
    lexicon.push_back(x);
  }
}

const std::vector<Sealib::Bitset<unsigned char>> &Sealib::DyckWordLexicon::getLexicon() {
  return lexicon;
}
unsigned int Sealib::DyckWordLexicon::getWordLength() {
  return wordLength;
}
