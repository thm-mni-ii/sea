#ifndef SEALIB_TIKZARRAY_H_
#define SEALIB_TIKZARRAY_H_

#include <string>
#include <sealib/tikzelement.h>
#include <vector>

namespace SealibVisual { class TikzArray; }
std::ostream &operator<<(std::ostream &, const SealibVisual::TikzArray &);

namespace SealibVisual {
/**
 * Abstract class representing a single Element in a TikzPicture.
 */
class TikzArray{
 private:
    std::vector<std::string> content;
    std::string name;
    std::string options;
    bool showIndices;

    static const std::string defaultName;
    static const std::string defaultOptions;

 public:
    explicit TikzArray(const std::vector<std::string> &content,
                       std::string name = TikzArray::defaultName,
                       std::string options = TikzArray::defaultOptions,
                       bool showIndices = false);

    std::string toString() const;
    friend std::ostream &(::operator<<)(std::ostream &os, const TikzArray &array);
};
}  // namespace SealibVisual
#endif //SEALIB_TIKZARRAY_H_
