#include "sealibvisual/tikzstack.h"
using SealibVisual::TikzStack;

TikzStack::TikzStack(const std::vector<std::string> &content, std::string name,
                     std::string options, bool showIndices)
    : TikzArray(content, name, options, showIndices) {}

std::ostream &TikzStack::out(std::ostream &os) const {
  os << "\\matrix"
     << "(" << name << ")"
     << "[" << options << "]";

  // array content
  os << "{\n\t";
  for (int i = content.size() - 1; i >= 0; i--) {
    if (showIndices) {
      os << i;
      os << " \\& ";
    }
    os << content[i];
    os << " \\\\ ";
  }
  os << std::endl
     << "}"
     << ";" << std::endl;
  // array content end

  return os;
}
