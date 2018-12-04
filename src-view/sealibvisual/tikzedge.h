#ifndef SEALIBVISUAL_TIKZEDGE_H_
#define SEALIBVISUAL_TIKZEDGE_H_

#include <string>
#include <vector>
#include "./tikzelement.h"

namespace SealibVisual {
/**
 * Representation of a graph edge. Inserting an edge will generate Tikz code
 * like: \path (A) edge[...] (B);
 */
class TikzEdge : public TikzElement {
 public:
    TikzEdge() = default;
    TikzEdge(const std::string &_from, const std::string &_to,
             const std::string &_options = "--")
        : from(_from), to(_to), options(_options) {}
    const std::string &getFrom() const { return from; }
    const std::string &getTo() const { return to; }
    const std::string &getOptions() const { return options; }

    void setOptions(const std::string &_options) { options = _options; }

 protected:
    std::ostream &out(std::ostream &os) const override;

 private:
    std::string from;
    std::string to;
    std::string options;
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZEDGE_H_
