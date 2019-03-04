#ifndef SEALIB_CDBITUTIL_H_
#define SEALIB_CDBITUTIL_H_

#define TWO 2
#define FOUR 4
#define EIGHT 8
#define SIXTEEN 16
#define THIRTYTWO 32

#define XOR_2 12297829382473034410UL
#define XOR_4 14757395258967641292UL
//#define XOR_4 9838263505978427528UL
#define XOR_8 17361588704580268272UL
//#define XOR_8 9259542123273814144UL
#define XOR_16 18374966859414961920UL
//#define XOR_16 9223512776490647552UL
#define XOR_32 18446462603027742720UL
//#define XOR_32 9223372039002259456UL

#define SHIFT_OFFSET 1UL

const unsigned long int WORDSIZE = sizeof(unsigned long int) * 8;

namespace Sealib {

class CdBitUtil {
 private:
    static unsigned long int getXor(unsigned long int colorFieldSize);
    static unsigned long int getMask(unsigned long int colorFieldSize);
    static unsigned long int foldWord(unsigned long int _word,
                                      unsigned long int colorFieldSize);
    static unsigned long int generateXorWord(unsigned long int color,
                                             unsigned long int colorFieldSize,
                                             unsigned long int word);

 public:
    static long int cdXorLoop(unsigned long int _word, unsigned long int color,
                              unsigned long int colorFieldSize);
    static long int cdXor(unsigned long int _word, unsigned long int color,
                          unsigned long int colorFieldSize);
    static unsigned long int cdColorIndices(unsigned long int value,
                                            unsigned long int color,
                                            unsigned long int colorFieldSize);
};

}  // namespace Sealib
#endif  // SEALIB_CDBITUTIL_H_
