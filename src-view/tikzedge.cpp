#include "sealibvisual/tikzedge.h"
std::ostream &SealibVisual::TikzEdge::out(std::ostream &os) const {
    return os << " (" << from << ") "
              << "edge"
              << " [" << options << "] "
              << "(" << to << ")";
}
