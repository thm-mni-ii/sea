#include <sealib/tikzstyle.h>

SealibVisual::TikzStyle::TikzStyle(std::string name, std::string options) :
    name(name), options(options) {
}

std::ostream &SealibVisual::TikzStyle::out(std::ostream &os) const {
    return os << "\tikzstyle" <<
              "{" << name << "}" <<
              "=" <<
              "[" << options << "]" <<
              std::endl;
}
const std::string &SealibVisual::TikzStyle::getName() const {
    return name;
}
void SealibVisual::TikzStyle::setName(const std::string &name) {
    TikzStyle::name = name;
}
const std::string &SealibVisual::TikzStyle::getOptions() const {
    return options;
}
void SealibVisual::TikzStyle::setOptions(const std::string &options) {
    TikzStyle::options = options;
}
