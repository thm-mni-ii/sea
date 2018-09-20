#include <sealib/tikzarray.h>
#include <utility>
#include <sstream>

SealibVisual::TikzArray::TikzArray(
    const std::vector<std::string> &content,
    const std::string &name,
    const std::string &options,
    bool showIndices) :
    content(content), name(name), options(options), showIndices(showIndices) {}

std::string SealibVisual::TikzArray::toString() const {
    std::stringstream ss;
    ss << this;
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const SealibVisual::TikzArray &array) {
    os << "\\matrix" <<
       "(" << array.name << ")" <<
       "[" << SealibVisual::TikzArray::defaultOptions << ", " << array.options << "]";

    // array content
    os << "{\n";
    if(array.showIndices) {
        for(unsigned int i = 0; i < array.content.size(); i++) {
            os << i;
            if(i < array.content.size() - 1) {
                os << " \\& ";
            } else {
                os << " \\\\\n";
            }
        }
    }
    for(unsigned int i = 0; i < array.content.size(); i++) {
        os << array.content[i];
        if(i < array.content.size() - 1) {
            os << " \\& ";
        } else {
            os << " \\\\\n";
        }
    }
    os << "\n};";
    // array content end
    return os;
}
