#ifndef SEALIBVISUAL_TIKZDRAW_H_
#define SEALIBVISUAL_TIKZDRAW_H_

#include <string>
#include <vector>
#include "./tikzelement.h"

namespace SealibVisual {
/**
 * Used for defining a custom style that can then be used via the given name.
 * Add to a document before using.
 */
class TikzDraw : public TikzElement {
 private:
    std::vector<std::string> points;
    std::vector<std::string> edges;
    std::string options;

 public:
    explicit TikzDraw(const std::vector<std::string> &_points,
                      const std::vector<std::string> &_edges,
                      std::string _options = "")
        : points(_points), edges(_edges), options(_options) {}

    std::ostream &out(std::ostream &os) const override;

    const std::string &getOptions() const;
    std::vector<std::string> &getPoints();
    std::vector<std::string> &getEdges();
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZDRAW_H_
