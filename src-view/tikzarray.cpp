#include <sealibvisual/tikzarray.h>
#include <utility>
#include <sstream>

SealibVisual::TikzArray::TikzArray(
    const std::vector<std::string> &content,
    std::string name,
    std::string options,
    bool showIndices) :
    content(content), name(name), options(options), showIndices(showIndices) {
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
    os << std::endl <<  "}" << ";" << std::endl;
    // array content end

    return os;
}

const std::string &SealibVisual::TikzArray::getName() const {
    return name;
}

void SealibVisual::TikzArray::setName(const std::string &name) {
    TikzArray::name = name;
}

const std::string &SealibVisual::TikzArray::getOptions() const {
    return options;
}

void SealibVisual::TikzArray::setOptions(const std::string &options) {
    TikzArray::options = options;
}

std::vector<std::string> &SealibVisual::TikzArray::getContent() {
    return content;
}

const std::vector<std::string> &SealibVisual::TikzArray::getContent() const {
    return content;
}
