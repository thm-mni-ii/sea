#include <sealibvisual/tikzarray.h>
#include <sealibvisual/tikzdocument.h>
#include <sealibvisual/tikzgenerator.h>
#include <sealibvisual/tikzgraph.h>
#include <sealibvisual/tikznode.h>
#include <sealibvisual/tikzpicture.h>

#include <sealib/basicgraph.h>
#include <sealib/bitset.h>
#include <sealib/compactarray.h>

#include <tuple>

namespace Sealib {
typedef Bitset<unsigned char> bitset_t;
typedef BasicGraph basicgraph_t;
}

namespace SealibVisual {

template <>
std::shared_ptr<SealibVisual::TikzElement>
SealibVisual::TikzGenerator::generateTikzElement<Sealib::bitset_t>(
    const Sealib::bitset_t &t, const std::string name) {
  std::vector<std::string> arr(t.size());

  for (unsigned int i = 0; i < t.size(); i++) {
    arr[i] = t[i] ? "1" : "0";
  }
  std::shared_ptr<TikzPicture> inner(new TikzPicture(
      "array/.style={"
      "matrix of nodes,"
      "ampersand replacement=\\&,"
      "nodes={draw, minimum size=7mm, fill=green!30},"
      "column sep=-\\pgflinewidth, row sep=0.5mm, nodes in empty cells,\n"
      "\t\trow 1/.style={nodes={draw=none, fill=none, minimum size=5mm}}}"));

  std::shared_ptr<TikzElement> tikzArray(
      new TikzArray(arr, "array", "array", true));
  inner->add(tikzArray);

  std::shared_ptr<TikzPicture> outer(new TikzPicture());
  std::shared_ptr<TikzNode> node1(
      new TikzNode(name, "anchor=west", inner->toString()));
  outer->add(node1);
  std::shared_ptr<TikzNode> node2(
      new TikzNode("label", "anchor=west", name + ":", "(0.1,1)"));
  outer->add(node2);

  return outer;
}

template <>
std::shared_ptr<SealibVisual::TikzElement>
SealibVisual::TikzGenerator::generateTikzElement<Sealib::bitset_t>(
    const Sealib::bitset_t &t) {
  return generateTikzElement(t, "bitset");
}

template <>
std::shared_ptr<SealibVisual::TikzElement>
SealibVisual::TikzGenerator::generateTikzElement<Sealib::basicgraph_t>(
    const Sealib::basicgraph_t &t) {
  using std::string;
  using std::shared_ptr;
  using std::tuple;
  using std::make_tuple;
  using std::to_string;
  shared_ptr<SealibVisual::TikzGraph> graph(
      new SealibVisual::TikzGraph(t.getOrder()));

  for (auto &a : graph->getNodes()) {
    a.setOptions("draw, circle");
  }

  for (unsigned int i = 0; i < t.getOrder(); i++) {
    for (const auto &a : t.getNode(i).getAdj()) {
      tuple<std::string, std::string> key =
          make_tuple(to_string(i), to_string(a.vertex));
      tuple<std::string, std::string> keyReverse =
          make_tuple(to_string(a.vertex), to_string(i));

      if (!graph->containsEdge(key) && !graph->containsEdge(keyReverse)) {
        graph->addEdge(key);
      }
    }
  }

  return graph;
}

using Sealib::CompactArray;
using SealibVisual::TikzElement;
using SealibVisual::TikzPicture;
template <>
std::shared_ptr<TikzElement> TikzGenerator::generateTikzElement<CompactArray>(
    const CompactArray &p) {
  CompactArray &c = const_cast<CompactArray &>(p);
  std::shared_ptr<TikzPicture> va(new TikzPicture(
      "array/.style={"
      "matrix of nodes,"
      "ampersand replacement=\\&,"
      "nodes={draw, minimum size=7mm, fill=white},"
      "column sep=-\\pgflinewidth, row sep=0.5mm, nodes in empty cells,"
      "row 1/.style={nodes={draw=none, fill=none, minimum size=5mm}}}"));

  std::vector<std::string> u;
  for (unsigned a = 0; a < c.size(); a++) {
    u.push_back(std::to_string(c.get(a)));
  }

  std::shared_ptr<TikzElement> array(new TikzArray(u, "C", "array", true));
  va->add(array);
  std::shared_ptr<TikzNode> node(
      new TikzNode("C", "anchor=west", va->toString()));
  return node;
}
}  // namespace SealibVisual
