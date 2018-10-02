#ifndef SEALIBVISUAL_TIKZGENERATOR_H_
#define SEALIBVISUAL_TIKZGENERATOR_H_

#include <sealibvisual/tikzelement.h>
#include <sealibvisual/tikzgraph.h>
#include <sealibvisual/tikznode.h>
#include <sealibvisual/tikzpicture.h>

#include <sealib/basicgraph.h>
#include <sealib/bitset.h>
#include <sealib/compactarray.h>
#include <memory>
#include <string>

namespace SealibVisual {
class TikzGenerator {
 public:
  TikzGenerator() = delete;

  static std::shared_ptr<TikzPicture> generateTikzElement(
      const Sealib::Bitset<unsigned char> &, const std::string name);
  static std::shared_ptr<TikzPicture> generateTikzElement(
      const Sealib::Bitset<unsigned char> &);

  static std::shared_ptr<TikzGraph> generateTikzElement(
      const Sealib::BasicGraph &);

  // Needs tikz libraries: matrix, positioning
  static std::shared_ptr<TikzPicture> generateTikzElement(
      Sealib::CompactArray &, std::string name = "");

  static std::shared_ptr<TikzPicture> generateTikzElement(
      std::vector<unsigned int> &, std::string name = "",
      bool vertical = false);
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZGENERATOR_H_
