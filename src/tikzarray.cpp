#include <sealib/tikzarray.h>
#include <utility>
#include <sstream>

const std::string SealibVisual::TikzArray::defaultName = "array";
const std::string SealibVisual::TikzArray::defaultOptions = "matrix of nodes, ampersand replacement=\\&";

SealibVisual::TikzArray::TikzArray(
    const std::vector<std::string> &content_,
    std::string name_,
    std::string options_,
    bool showIndices_) :
    content(content_), name(std::move(name_)), options(std::move(options_)), showIndices(showIndices_) {
}

std::string SealibVisual::TikzArray::toString() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}
std::ostream &SealibVisual::TikzArray::out(std::ostream &os) const {
    os << "\\matrix" <<
       "(" << name << ")" <<
       "[" << options << "]";

    // array content
    os << "{\n\t";
    if (showIndices) {
        for (unsigned int i = 0; i < content.size(); i++) {
            os << i;
            if (i < content.size() - 1) {
                os << " \\& ";
            } else {
                os << " \\\\";
            }
        }
        os << "\n";
    }
    os << "\t";
    for (unsigned int i = 0; i < content.size(); i++) {
        os << content[i];
        if (i < content.size() - 1) {
            os << " \\& ";
        } else {
            os << " \\\\";
        }
    }
    os << "\n};";
    // array content end

    return os;
}
