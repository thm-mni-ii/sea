#include <sealib/tikzelement.h>

std::ostream &operator<<(std::ostream &os, const SealibVisual::TikzElement &tikzElement) {
    return tikzElement.out(os);
}

std::string SealibVisual::TikzElement::toString() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}
