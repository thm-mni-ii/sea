#ifndef SEALIBVISUAL_EXAMPLES_H_
#define SEALIBVISUAL_EXAMPLES_H_
#include <sealibvisual/tikzdocument.h>
#include <string>

namespace SealibVisual {
class Examples {
 public:
  static void visualBFS(std::string filename = "example.tex");

  static void visualDFS(std::string filename = "example.tex");
};
}
#endif  // SEALIBVISUAL_EXAMPLES_H_
