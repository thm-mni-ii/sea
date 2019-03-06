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

#include <cstdint>

const uint64_t WORDSIZE = sizeof(uint64_t) * 8;

namespace Sealib {

class CdBitUtil {
 private:
    static uint64_t getXor(uint64_t colorFieldSize);
    static uint64_t getMask(uint64_t colorFieldSize);
    static uint64_t foldWord(uint64_t _word,
                                      uint64_t colorFieldSize);
    static uint64_t generateXorWord(uint64_t color,
                                             uint64_t colorFieldSize,
                                             uint64_t word);

 public:
    static int64_t cdXorLoop(uint64_t _word, uint64_t color,
                              uint64_t colorFieldSize);
    static int64_t cdXor(uint64_t _word, uint64_t color,
                          uint64_t colorFieldSize);
    static uint64_t cdColorIndices(uint64_t value,
                                            uint64_t color,
                                            uint64_t colorFieldSize);
};

}  // namespace Sealib
#endif  // SEALIB_CDBITUTIL_H_
