#ifndef SEALIB_TIKZPICTURE_H_
#define SEALIB_TIKZPICTURE_H_
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <sealib/tikzelement.h>
#include <memory>

namespace SealibVisual {
class TikzPicture : public TikzElement {
 private:
    std::vector<std::shared_ptr<TikzElement>> elements;
    std::string options;

 public:
    explicit TikzPicture(std::string options = "");
    void add(std::shared_ptr<TikzElement> tikzElement);

 protected:
    std::ostream &out(std::ostream &os) const override;
};
}  // namespace SealibVisual
#endif //SEALIB_TIKZPICTURE_H_
