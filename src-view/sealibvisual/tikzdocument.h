#ifndef SEALIBVISUAL_TIKZDOCUMENT_H_
#define SEALIBVISUAL_TIKZDOCUMENT_H_

#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "./tikzelement.h"

namespace SealibVisual {
/**
 * The TikzDocument class contains several TEX elements or Tikz pictures.
 */
class TikzDocument {
 public:
    /**
     * Creates a new TikzDocument.
     * @param _filename Output file name
     * @param _tikzLibraries Tikz libraries to use (generates \usetikzlibrary
     * statement)
     * @param _gdLibraries Tikz graph drawing libraries to use (generates
     * \usegdlibrary statement)
     * @param _lualatex Compile with LuaLaTex? (needed for graph layouts)
     * @param _mode output format to generate ("standalone": a freestanding
     * (lua)latex document, to animate the resulting document, use 'convert' on
     * the PDF; "beamer": (lua)latex code to include in a beamer presentation)
     */
    explicit TikzDocument(std::string _filename,
                          std::string _tikzLibraries = "",
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
    /**
     * Closes the TikzDocument and the output stream.
     */
    void close();

    bool isOpen();

    /**
     * The following methods add an element or string to the current document.
     */
    void add(const SealibVisual::TikzElement *element);
    void add(const SealibVisual::TikzElement &element);
    void add(std::shared_ptr<SealibVisual::TikzElement> element);
    void add(const std::string &line);
    void add(const char *line);

    /**
     * Starts a block. A block should contain one or more TikzPictures that
     * belong together.
     */
    void beginBlock();
    /**
     * Ends the most recently started block.
     */
    void endBlock();

 private:
    std::string filename;
    std::ofstream file;
    std::string tikzLibraries;
    std::string gdLibraries;
    bool lualatex;
    std::string mode;
    const std::string blockName = "tikzpicture";
    uint64_t slide = 1;

    void initialize();
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZDOCUMENT_H_
