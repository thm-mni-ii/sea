#include "sealibvisual/tikzedge.h"
SealibVisual::TikzEdge::TikzEdge(
    const std::string &from,
    const std::string &to,
    const std::string &options)
    : from(from), to(to), options(options) {}

std::ostream &SealibVisual::TikzEdge::out(std::ostream &os) const {
    return os << " (" << from << ") " << "edge" <<
              " [" << options << "] " <<
              "(" << to << ")";
}
const std::string &SealibVisual::TikzEdge::getFrom() const {
    return from;
}
const std::string &SealibVisual::TikzEdge::getTo() const {
    return to;
}
const std::string &SealibVisual::TikzEdge::getOptions() const {
    return options;
}
void SealibVisual::TikzEdge::setOptions(const std::string &options) {
    TikzEdge::options = options;
}
