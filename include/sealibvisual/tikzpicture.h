#ifndef SEALIBVISUAL_TIKZPICTURE_H_
#define SEALIBVISUAL_TIKZPICTURE_H_

#include <sealibvisual/tikzelement.h>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
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
#endif  // SEALIBVISUAL_TIKZPICTURE_H_
