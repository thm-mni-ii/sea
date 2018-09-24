#ifndef SEALIB_TIKZNODE_H_
#define SEALIB_TIKZNODE_H_

#include <sealibvisual/tikzelement.h>
#include <string>

namespace SealibVisual {
/**
 * Abstract class representing a single Element in a TikzPicture.
 */
class TikzNode : public TikzElement {
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

    TikzNode() = default;

    std::ostream &out(std::ostream &os) const override;

    const std::string &getName() const;
    void setName(const std::string &name);
    const std::string &getOptions() const;
    void setOptions(const std::string &options);
    const std::string &getCoordinate() const;
    void setCoordinate(const std::string &coordinate);
    const std::string &getContent() const;
    void setContent(const std::string &content);
    void setContent(const TikzElement &element);
};
}  // namespace SealibVisual
#endif  // SEALIB_TIKZNODE_H_
