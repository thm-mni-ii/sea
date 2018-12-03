#ifndef SEALIBVISUAL_TIKZNODE_H_
#define SEALIBVISUAL_TIKZNODE_H_

#include "./tikzelement.h"
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
    explicit TikzNode(const std::string &_name,
                      const std::string &_options = "",
                      const std::string &_content = "",
                      const std::string &_coordinate = "")
      : name(_name),
        options(_options),
        content(_content),
        coordinate(_coordinate) {}

    TikzNode() = default;

    std::ostream &out(std::ostream &os) const override;

    std::string const &getName() const { return name; }
    void setName(const std::string &_name) { name=_name; }
    std::string const &getOptions() const { return options; }
    void setOptions(const std::string &_options) { options=_options; }
    std::string const &getCoordinate() const { return coordinate; }
    void setCoordinate(const std::string &_coordinate) { coordinate=_coordinate; }
    std::string const &getContent() const { return content; }
    void setContent(const std::string &_content) { content=_content; }
    void setContent(const TikzElement &_element) { content=_element.toString(); }
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZNODE_H_
