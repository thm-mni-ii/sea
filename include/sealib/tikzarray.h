#ifndef SEALIB_TIKZARRAY_H_
#define SEALIB_TIKZARRAY_H_

#include <sealib/tikzelement.h>
#include <string>
#include <vector>

namespace SealibVisual {
/**
 * Abstract class representing a single Element in a TikzPicture.
 */
class TikzArray : public TikzElement {
 private:
    std::vector<std::string> content;
    std::string name;
    std::string options;
    bool showIndices;

 public:
    explicit TikzArray(const std::vector<std::string> &content,
                       std::string name = "array",
                       std::string options = "matrix of nodes, ampersand replacement=\\&",
                       bool showIndices = false);

    std::ostream &out(std::ostream &os) const override;

    std::vector<std::string> &getContent();
    const std::vector<std::string> &getContent() const;
    const std::string &getName() const;
    void setName(const std::string &name);
    const std::string &getOptions() const;
    void setOptions(const std::string &options);
};
}  // namespace SealibVisual
#endif  // SEALIB_TIKZARRAY_H_