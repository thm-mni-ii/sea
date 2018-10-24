#ifndef SEALIBVISUAL_TIKZGENERATOR_H_
#define SEALIBVISUAL_TIKZGENERATOR_H_

#include "./tikzelement.h"
#include "./tikzgraph.h"
#include "./tikznode.h"
#include "./tikzpicture.h"

#include <sealib/basicgraph.h>
#include <sealib/bitset.h>
#include <sealib/compactarray.h>
#include <memory>
#include <string>
#include <vector>

namespace SealibVisual {
class TikzGenerator {
 public:
  TikzGenerator() = delete;

  static std::shared_ptr<TikzPicture> generateTikzElement(
      const Sealib::Bitset<unsigned char> &, const std::string name);
  static std::shared_ptr<TikzPicture> generateTikzElement(
      const Sealib::Bitset<unsigned char> &);

  static std::shared_ptr<TikzGraph> generateTikzElement(
      const Sealib::BasicGraph &, bool directed = false);

  // Needs tikz libraries: matrix, positioning
  static std::shared_ptr<TikzPicture> generateTikzElement(
      Sealib::CompactArray &, std::string name = "",
      std::string positionOpts = "");

  static std::shared_ptr<TikzPicture> generateTikzElement(
      std::vector<unsigned int> &, std::string name = "", bool vertical = false,
      std::string positionOpts = "");
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZGENERATOR_H_
