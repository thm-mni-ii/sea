#include <sealib/measurement/bytecounter.h>

size_t &Sealib::ByteCounter::get() {
    static size_t cnt;
    return cnt;
}
