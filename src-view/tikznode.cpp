#include "sealibvisual/tikznode.h"
#include <sstream>
#include <utility>
#include <vector>

std::ostream &SealibVisual::TikzNode::out(std::ostream &os) const {
    std::string coordinateString;
    if (!coordinate.empty()) {
        coordinateString = " at " + coordinate + " ";
    }
    return os << "\\node"
              << "(" << name << ")"
              << "[" << options << "]" << coordinateString << "{" << content
              << "}"
              << ";" << std::endl;
}
