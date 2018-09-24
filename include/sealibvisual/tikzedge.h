#ifndef SEALIB_TIKZEDGE_H_
#define SEALIB_TIKZEDGE_H_

#include <sealibvisual/tikzelement.h>
#include <string>
#include <vector>

namespace SealibVisual {
class TikzEdge : public TikzElement {
 private:
    std::string from;
    std::string to;
    std::string options;

 public:
    TikzEdge() = default;
    TikzEdge(const std::string &from, const std::string &to, const std::string &options = "--");
    const std::string &getFrom() const;
    const std::string &getTo() const;
    const std::string &getOptions() const;

    void setOptions(const std::string &options);
 protected:
    std::ostream &out(std::ostream &os) const override;
};
}  // namespace SealibVisual
#endif  // SEALIB_TIKZEDGE_H_
