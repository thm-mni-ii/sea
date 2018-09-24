#ifndef SEALIB_TIKZDOCUMENT_H_
#define SEALIB_TIKZDOCUMENT_H_

#include <include/sealibvisual/tikzelement.h>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <memory>

namespace SealibVisual {
class TikzDocument {
 private:
    std::string filename;
    std::string tikzLibraries;
    std::string gdLibraries;
    bool lualatex;
    std::ofstream file;

    void initialize();

 public:
    explicit TikzDocument(std::string filename,
                          std::string tikzLibraries = "",
                          std::string gdLibraries = "",
                          bool lualatex = false);
    void close();

    bool isOpen();

    void add(const SealibVisual::TikzElement *element);
    void add(const SealibVisual::TikzElement &element);
    void add(std::shared_ptr<SealibVisual::TikzElement> element);
    void add(const std::string &line);
    void add(const char *line);
};
}  // namespace SealibVisual
#endif  // SEALIB_TIKZDOCUMENT_H_
