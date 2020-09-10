#ifndef SEALIBVISUAL_TIKZDRAW_H_
#define SEALIBVISUAL_TIKZDRAW_H_

#include <string>
#include <utility>
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
    explicit TikzDraw(std::vector<std::string> _points,
                      std::vector<std::string> _edges,
                      std::string _options = "")
        : points(std::move(_points)), edges(std::move(_edges)), options(std::move(_options)) {}

    std::ostream &out(std::ostream &os) const override;

    [[nodiscard]] const std::string &getOptions() const;
    std::vector<std::string> &getPoints();
    std::vector<std::string> &getEdges();
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZDRAW_H_
