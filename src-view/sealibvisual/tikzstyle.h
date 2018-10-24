#ifndef SEALIBVISUAL_TIKZSTYLE_H_
#define SEALIBVISUAL_TIKZSTYLE_H_

#include "./tikzelement.h"
#include <string>

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
    explicit TikzStyle(std::string name,
                       std::string options);

    std::ostream &out(std::ostream &os) const override;

    const std::string &getName() const;
    void setName(const std::string &name);
    const std::string &getOptions() const;
    void setOptions(const std::string &options);
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZSTYLE_H_
