#ifndef SEALIBVISUAL_TIKZSTACK_H_
#define SEALIBVISUAL_TIKZSTACK_H_
#include "sealibvisual/tikzarray.h"
namespace SealibVisual {
/**
* Visualizes a vertical array that grows upwards.
*
* @author Simon Heuser
*/
class TikzStack : public TikzArray {
 public:
  explicit TikzStack(
      const std::vector<std::string> &content, std::string name = "array",
      std::string options = "matrix of nodes, ampersand replacement=\\&",
      bool showIndices = false);
  std::ostream &out(std::ostream &os) const override;
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZSTACK_H_
