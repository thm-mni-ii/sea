#ifndef SEALIBVISUAL_TIKZGENERATOR_H_
#define SEALIBVISUAL_TIKZGENERATOR_H_

#include "./tikzelement.h"
#include "./tikzgraph.h"
#include "./tikznode.h"
#include "./tikzpicture.h"

#include <sealib/collection/bitset.h>
#include <sealib/collection/compactarray.h>
#include <sealib/graph/directedgraph.h>
#include <sealib/graph/undirectedgraph.h>
#include <memory>
#include <string>
#include <vector>

namespace SealibVisual {
/**
 * Class to generate Tikz representations of given input objects.
 */
class TikzGenerator {
 public:
    TikzGenerator() = delete;

    static std::shared_ptr<TikzPicture> generateTikzElement(
        const Sealib::Bitset<uint8_t> &, const std::string &name);
    static std::shared_ptr<TikzPicture> generateTikzElement(
        const Sealib::Bitset<uint8_t> &);

    static std::shared_ptr<TikzGraph> generateTikzElement(
        Sealib::UndirectedGraph const &g);
    static std::shared_ptr<TikzGraph> generateTikzElement(
        Sealib::DirectedGraph const &g);
    static std::shared_ptr<TikzGraph> generateTikzElement(
        Sealib::Graph const &g);

    // Needs tikz libraries: matrix, positioning
    static std::shared_ptr<TikzPicture> generateTikzElement(
        Sealib::CompactArray *, size_t, std::string name = "",
        std::string positionOpts = "");

    static std::shared_ptr<TikzPicture> generateTikzElement(
        std::vector<uint64_t> &, std::string name = "",
        bool vertical = false, std::string positionOpts = "");
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZGENERATOR_H_
