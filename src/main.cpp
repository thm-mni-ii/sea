
#include <sealib/graphcreator.h>
#include "sealib/dyckword.h"
#include <iostream>

using namespace std;

int main() {
    DyckWord w = DyckWord(8);
    // (()(()))
    // 01234567

    w.flip(0);
    w.flip(1);
    w.flip(3);
    w.flip(4);

    for(size_t i = 0; i < 8; i++) {
        cout << w.at(i);
    }
}