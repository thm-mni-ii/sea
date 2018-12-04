#include "sealibvisual/tikzarray.h"
#include <sstream>
#include <utility>

std::ostream &SealibVisual::TikzArray::out(std::ostream &os) const {
    os << "\\matrix"
       << "(" << name << ")"
       << "[" << options << "]";

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
    os << std::endl
       << "}"
       << ";" << std::endl;
    // array content end

    return os;
}
