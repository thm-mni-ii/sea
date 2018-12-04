#include "sealibvisual/tikzstyle.h"

std::ostream &SealibVisual::TikzStyle::out(std::ostream &os) const {
    return os << "\tikzstyle"
              << "{" << name << "}"
              << "="
              << "[" << options << "]" << std::endl;
}
