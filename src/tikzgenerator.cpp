#include <sealib/tikzgenerator.h>
#include <sealib/bitset.h>
#include <sealib/tikzarray.h>
#include <sealib/tikznode.h>

typedef Sealib::Bitset<unsigned char> bitset_t;

template<>
std::unique_ptr<SealibVisual::TikzElement>
SealibVisual::TikzGenerator::generateTikzElement<bitset_t>(const bitset_t &t, const std::string name) {
    std::vector<std::string> arr(t.size());

    for (unsigned int i = 0; i < t.size(); i++) {
        arr[i] = t[i] ? "1" : "0";
    }
    return std::unique_ptr<TikzElement>(new TikzNode(name, "",TikzArray(arr).toString(),""));
}

template<>
std::unique_ptr<SealibVisual::TikzElement>
SealibVisual::TikzGenerator::generateTikzElement<bitset_t>(const bitset_t &t) {
    generateTikzElement(t, "bitset");
}
