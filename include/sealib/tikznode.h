#ifndef SEALIB_TIKZNODE_H_
#define SEALIB_TIKZNODE_H_

#include <string>
#include <sealib/tikzelement.h>
namespace SealibVisual{ class TikzNode; }
std::ostream& operator<<(std::ostream&, const SealibVisual::TikzNode&);

namespace SealibVisual {
/**
 * Abstract class representing a single Element in a TikzPicture.
 */
class TikzNode : TikzElement {
 private:
    std::string name;
    std::string options;
    std::string content;
    std::string coordinate;
 public:
    explicit TikzNode(const std::string &name,
                      const std::string &options = "",
                      const std::string &content = "",
                      const std::string &coordinate = "");

    std::string toString() const override;
    friend std::ostream& (::operator<<) (std::ostream& os, const TikzNode& node);
};
}  // namespace SealibVisual
#endif //SEALIB_TIKZNODE_H_
