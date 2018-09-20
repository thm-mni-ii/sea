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
    ss << this;
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const SealibVisual::TikzNode &node) {
    std::string coordinateString;
    if (node.coordinate.empty()) {
        coordinateString = "at " + node.coordinate + " ";
    }
    return os << "\\node" <<
              "(" << node.name << ")" <<
              "[" << node.options << "]" <<
              coordinateString <<
              "{" << node.content << "}";
}
