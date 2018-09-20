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
class TikzArray : TikzElement {
 private:
    std::vector<std::string> content;
    std::string name;
    std::string options;
    bool showIndices;

    const constexpr std::string defaultOptions = "matrix of nodes, ampersand replacement=\\&";

 public:
    explicit TikzArray(const std::vector<std::string> &content,
                       const std::string &name = "",
                       const std::string &options = "",
                       bool showIndices = false);

    std::string toString() const override;
    friend std::ostream &(::operator<<)(std::ostream &os, const TikzArray &array);
};
}  // namespace SealibVisual
#endif //SEALIB_TIKZARRAY_H_
