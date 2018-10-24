#ifndef SEALIBVISUAL_TIKZDOCUMENT_H_
#define SEALIBVISUAL_TIKZDOCUMENT_H_

#include "./tikzelement.h"
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
  std::string mode;
  const std::string blockName = "tikzpicture";

  void initialize();

 public:
  /**
   * @param mode output format to generate ("standalone": a freestanding
   * (lua)latex document, to animate the resulting document, use 'convert' on
   * the PDF; "beamer": (lua)latex code to include in a beamer presentation)
   */
  explicit TikzDocument(std::string filename, std::string tikzLibraries = "",
                        std::string gdLibraries = "", bool lualatex = false,
                        std::string mode = "standalone");
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
