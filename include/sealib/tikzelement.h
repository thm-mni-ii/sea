#ifndef SEALIB_TIKZELEMENT_H_
#define SEALIB_TIKZELEMENT_H_

#include <iostream>
namespace SealibVisual {
/**
 * Abstract class representing a single Element in a TikzPicture.
 */
class TikzElement {
 public:
    virtual std::string toString() const = 0;
};
}  // namespace SealibVisual
#endif //SEALIB_TIKZELEMENT_H_
