#ifndef SEALIB_TIKZDRAW_H_
#define SEALIB_TIKZDRAW_H_

#include <include/sealibvisual/tikzelement.h>
#include <string>
#include <vector>

namespace SealibVisual {
/**
 * Used for defining a custom style that can then be used via the given name.
 * Add to a document before using.
 */
class TikzDraw : public TikzElement {
 private:
    std::string options;
    std::vector<std::string> points;
    std::vector<std::string> edges;

 public:
    explicit TikzDraw(const std::vector<std::string> &points,
                      const std::vector<std::string> &edges,
                      std::string options = "");

    std::ostream &out(std::ostream &os) const override;

    const std::string &getOptions() const;
    std::vector<std::string> &getPoints();
    std::vector<std::string> &getEdges();
};
}  // namespace SealibVisual
#endif  // SEALIB_TIKZDRAW_H_
