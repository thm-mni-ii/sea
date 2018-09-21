#ifndef SEALIB_TIKZDOCUMENT_H_
#define SEALIB_TIKZDOCUMENT_H_

#include <sealib/tikzelement.h>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <memory>


namespace SealibVisual {
class TikzDocument {
 private:
    std::string filename;
    std::string libraries;
    std::ofstream file;
    void initialize();

 public:
    explicit TikzDocument(std::string filename, std::string libraries = "");
    void close();

    bool isOpen();

    void add(const SealibVisual::TikzElement *element);
    void add(std::shared_ptr<SealibVisual::TikzElement> element);

    std::ostream& operator<< (std::ostream& (*pf)(std::ostream&));
};
}  // namespace SealibVisual
#endif  // SEALIB_TIKZDOCUMENT_H_
