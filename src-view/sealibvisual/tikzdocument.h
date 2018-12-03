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
  std::ofstream file;
  std::string tikzLibraries;
  std::string gdLibraries;
  bool lualatex;
  std::string mode;
  const std::string blockName = "tikzpicture";

  void initialize();

 public:
  /**
   * @param mode output format to generate ("standalone": a freestanding
   * (lua)latex document, to animate the resulting document, use 'convert' on
   * the PDF; "beamer": (lua)latex code to include in a beamer presentation)
   */
  explicit TikzDocument(std::string _filename, std::string _tikzLibraries = "",
                        std::string _gdLibraries = "", bool _lualatex = false,
                        std::string _mode = "standalone") 
    : filename(_filename),
      file(_filename),
      tikzLibraries(_tikzLibraries),
      gdLibraries(_gdLibraries),
      lualatex(_lualatex),
      mode(_mode) {
        initialize();
      }
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
