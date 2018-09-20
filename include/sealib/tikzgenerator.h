#ifndef SEALIB_TIKZGENERATOR_H_
#define SEALIB_TIKZGENERATOR_H_

#include <sealib/tikzelement.h>
#include <memory>

namespace SealibVisual {
class TikzGenerator {
 public:
    TikzGenerator() = delete;

    template<class T>
    static std::unique_ptr<SealibVisual::TikzElement> generateTikzElement(const T &t, const std::string name);

    template<class T>
    static std::unique_ptr<SealibVisual::TikzElement> generateTikzElement(const T &t);
};
}  // namespace SealibVisual
#endif // SEALIB_TIKZGENERATOR_H_
