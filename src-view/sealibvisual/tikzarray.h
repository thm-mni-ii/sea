#ifndef SEALIBVISUAL_TIKZARRAY_H_
#define SEALIBVISUAL_TIKZARRAY_H_

#include "./tikzelement.h"
#include <string>
#include <vector>

namespace SealibVisual {
/**
 * Abstract class representing a single Element in a TikzPicture.
 */
class TikzArray : public TikzElement {
 protected:
    std::vector<std::string> content;
    std::string name;
    std::string options;
    bool showIndices;

 public:
    explicit TikzArray(const std::vector<std::string> &_content,
                       std::string _name = "array",
                       std::string _options = "matrix of nodes, ampersand replacement=\\&",
                       bool _showIndices = false)
        : content(_content), name(_name), options(_options), showIndices(_showIndices) {}

    std::ostream &out(std::ostream &os) const override;

    std::vector<std::string> &getContent() { return content; }
    const std::vector<std::string> &getContent() const { return content; }
    const std::string &getName() const { return name; }
    void setName(const std::string &_name) { name=_name; }
    const std::string &getOptions() const { return options; }
    void setOptions(const std::string &_options) { options=_options; }
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZARRAY_H_
