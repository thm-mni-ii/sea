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

std::ostream &operator<<(std::ostream &os, const SealibVisual::TikzArray &array) {
    os << "\\matrix" <<
       "(" << array.name << ")" <<
       "[" << array.options << "]";

    // array content
    os << "{\n\t";
    if(array.showIndices) {
        for(unsigned int i = 0; i < array.content.size(); i++) {
            os << i;
            if(i < array.content.size() - 1) {
                os << " \\& ";
            } else {
                os << " \\\\";
            }
        }
        os << "\n";
    }
    os << "\t";
    for(unsigned int i = 0; i < array.content.size(); i++) {
        os << array.content[i];
        if(i < array.content.size() - 1) {
            os << " \\& ";
        } else {
            os << " \\\\";
        }
    }
    os << "\n};";
    // array content end

    return os;
}
