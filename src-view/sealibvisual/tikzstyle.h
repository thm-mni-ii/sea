#ifndef SEALIBVISUAL_TIKZSTYLE_H_
#define SEALIBVISUAL_TIKZSTYLE_H_

#include <string>
#include "./tikzelement.h"

namespace SealibVisual {
/**
 * Used for defining a custom style that can then be used via the given name.
 * Add to a document before using.
 */
class TikzStyle : public TikzElement {
 private:
    std::string name;
    std::string options;

 public:
    explicit TikzStyle(std::string _name, std::string _options)
        : name(_name), options(_options) {}

    std::ostream &out(std::ostream &os) const override;

    const std::string &getName() const { return name; }
    void setName(const std::string &_name) { name = _name; }
    const std::string &getOptions() const { return options; }
    void setOptions(const std::string &_options) { options = _options; }
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZSTYLE_H_
