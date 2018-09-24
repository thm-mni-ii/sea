#ifndef SEALIB_TIKZELEMENT_H_
#define SEALIB_TIKZELEMENT_H_

#include <iostream>
#include <sstream>
#include <string>

namespace SealibVisual {
/**
 * Abstract class representing a single Element in a TikzPicture.
 */
class TikzElement {
 protected:
    virtual std::ostream &out(std::ostream &os) const = 0;
 public:
    virtual ~TikzElement() = default;
    virtual std::string toString() const;

    friend std::ostream &operator<<(std::ostream &os,
                                    const SealibVisual::TikzElement &tikzElement) {
        return tikzElement.out(os);
    }
};
}  // namespace SealibVisual
#endif  // SEALIB_TIKZELEMENT_H_
