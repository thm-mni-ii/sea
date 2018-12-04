#ifndef SEALIBVISUAL_TIKZARRAY_H_
#define SEALIBVISUAL_TIKZARRAY_H_

#include <string>
#include <vector>
#include "./tikzelement.h"

namespace SealibVisual {
/**
 * Representation of an array. Generates a Tikz \matrix.
 */
class TikzArray : public TikzElement {
 public:
    /**
     * Creates a new TikzArray.
     * @param _content Input data to use
     * @param _name Name of the matrix
     * @param _options Options for the matrix
     * @param _showIndices Should the indices be displayed?
     */
    explicit TikzArray(
        const std::vector<std::string> &_content, std::string _name = "array",
        std::string _options = "matrix of nodes, ampersand replacement=\\&",
        bool _showIndices = false)
        : content(_content),
          name(_name),
          options(_options),
          showIndices(_showIndices) {}

    std::ostream &out(std::ostream &os) const override;

    std::vector<std::string> &getContent() { return content; }
    const std::vector<std::string> &getContent() const { return content; }
    const std::string &getName() const { return name; }
    void setName(const std::string &_name) { name = _name; }
    const std::string &getOptions() const { return options; }
    void setOptions(const std::string &_options) { options = _options; }

 protected:
    std::vector<std::string> content;
    std::string name;
    std::string options;
    bool showIndices;
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZARRAY_H_
