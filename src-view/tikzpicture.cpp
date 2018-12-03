#include "sealibvisual/tikzpicture.h"
#include <sstream>
#include <utility>

std::ostream &SealibVisual::TikzPicture::out(std::ostream &os) const {
  os << "\\begin{" << (scope ? "scope" : "tikzpicture") << "}"
     << "[" << options << "]" << std::endl;
  for (const std::shared_ptr<TikzElement> &e : elements) {
    os << (*e);
  }
  os << "\\end{" << (scope ? "scope" : "tikzpicture") << "}" << std::endl;
  os << "%\\hspace{0cm}";
  return os;
}

void SealibVisual::TikzPicture::add(std::shared_ptr<TikzElement> tikzElement) {
  elements.emplace_back(tikzElement);
}
