#ifndef SEALIBVISUAL_TIKZSTACK_H_
#define SEALIBVISUAL_TIKZSTACK_H_

#include <string>
#include <vector>
#include "./tikzarray.h"

namespace SealibVisual {
/**
* Representation of a stack (a vertical array that grows upwards).
*/
class TikzStack : public TikzArray {
 public:
    /**
     * Creates a new TikzStack. (see TikzArray for help)
     */
    explicit TikzStack(
        const std::vector<std::string> &content, std::string name = "array",
        std::string options = "matrix of nodes, ampersand replacement=\\&",
        bool showIndices = false);
    TikzStack() = default;
    TikzStack(const TikzStack &) = default;
    TikzStack(TikzStack &&) noexcept = default;
    TikzStack& operator=(const TikzStack &) = default;
    // TikzStack& operator=(TikzStack &&) noexcept = default;
    virtual ~TikzStack() = default;

    std::ostream &out(std::ostream &os) const override;
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZSTACK_H_
