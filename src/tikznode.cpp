#include <sealib/tikznode.h>
#include <utility>
#include <sstream>
#include <vector>
SealibVisual::TikzNode::TikzNode(
    const std::string &name,
    const std::string &options,
    const std::string &content,
    const std::string &coordinate) :
    name(name),
    options(options),
    content(content),
    coordinate(coordinate) {}

std::string SealibVisual::TikzNode::toString() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}

std::ostream &SealibVisual::TikzNode::out(std::ostream &os) const {
    std::string coordinateString;
    if (!coordinate.empty()) {
        coordinateString = " at " + coordinate + " ";
    }
    return os << "\\node" <<
              "(" << name << ")" <<
              "[" << options << "]" <<
              coordinateString <<
              "{" << content << "}";
}
