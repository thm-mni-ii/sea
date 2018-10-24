#include "sealibvisual/tikzelement.h"

std::string SealibVisual::TikzElement::toString() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}
