#ifndef SEALIBVISUAL_TIKZDOCUMENT_H_
#define SEALIBVISUAL_TIKZDOCUMENT_H_

#include <sealibvisual/tikzelement.h>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace SealibVisual {
class TikzDocument {
 private:
  std::string filename;
  std::string tikzLibraries;
  std::string gdLibraries;
  bool lualatex;
  std::ofstream file;
  const std::string blockName = "tikzContainer";

  void initialize();

 public:
  // ! to animate the resulting document, use 'convert' on the PDF
  explicit TikzDocument(std::string filename, std::string tikzLibraries = "",
                        std::string gdLibraries = "", bool lualatex = false);
  void close();

  bool isOpen();

  void add(const SealibVisual::TikzElement *element);
  void add(const SealibVisual::TikzElement &element);
  void add(std::shared_ptr<SealibVisual::TikzElement> element);
  void add(const std::string &line);
  void add(const char *line);

  // Starts a block. All tikzpictures in a block will be put on the same page
  void beginBlock();
  // Ends a block.
  void endBlock();
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZDOCUMENT_H_
