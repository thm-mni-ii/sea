#include <sealibvisual/tikzpicture.h>
#include <utility>
#include <sstream>

std::ostream &SealibVisual::TikzPicture::out(std::ostream &os) const {
    os << "\\begin{tikzpicture}" <<
       "[" << options << "]" << std::endl;
    for (const std::shared_ptr<TikzElement> &e : elements) {
        os << (*e) << std::endl;
    }
    os << "\\end{tikzpicture}" << std::endl;
    return os;
}

SealibVisual::TikzPicture::TikzPicture(std::string options) :
    elements(),
    options(std::move(options)) {}

void SealibVisual::TikzPicture::add(std::shared_ptr<TikzElement> tikzElement) {
    elements.emplace_back(tikzElement);
}
