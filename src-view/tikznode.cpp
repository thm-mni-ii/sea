#include "sealibvisual/tikznode.h"
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

std::ostream &SealibVisual::TikzNode::out(std::ostream &os) const {
    std::string coordinateString;
    if (!coordinate.empty()) {
        coordinateString = " at " + coordinate + " ";
    }
    return os << "\\node" <<
              "(" << name << ")" <<
              "[" << options << "]" <<
              coordinateString <<
              "{" << content << "}" << ";" << std::endl;
}

const std::string &SealibVisual::TikzNode::getName() const {
    return name;
}

void SealibVisual::TikzNode::setName(const std::string &name) {
    TikzNode::name = name;
}

const std::string &SealibVisual::TikzNode::getOptions() const {
    return options;
}

void SealibVisual::TikzNode::setOptions(const std::string &options) {
    TikzNode::options = options;
}

const std::string &SealibVisual::TikzNode::getCoordinate() const {
    return coordinate;
}

void SealibVisual::TikzNode::setCoordinate(const std::string &coordinate) {
    TikzNode::coordinate = coordinate;
}
const std::string &SealibVisual::TikzNode::getContent() const {
    return content;
}

void SealibVisual::TikzNode::setContent(const std::string &content) {
    TikzNode::content = content;
}

void SealibVisual::TikzNode::setContent(const TikzElement &element) {
    TikzNode::content = element.toString();
}
