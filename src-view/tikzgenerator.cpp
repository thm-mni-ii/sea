#include "sealibvisual/tikzgenerator.h"

#include <sstream>
#include <tuple>

#include "sealibvisual/tikzarray.h"
#include "sealibvisual/tikzdocument.h"
#include "sealibvisual/tikzgraph.h"
#include "sealibvisual/tikznode.h"
#include "sealibvisual/tikzpicture.h"
#include "sealibvisual/tikzstack.h"

#include "sealib/basicgraph.h"
#include "sealib/bitset.h"
#include "sealib/compactarray.h"

namespace Sealib {
typedef Bitset<unsigned char> bitset_t;
typedef UndirectedGraph basicgraph_t;
}

namespace SealibVisual {

std::shared_ptr<SealibVisual::TikzPicture>
SealibVisual::TikzGenerator::generateTikzElement(
    const Sealib::Bitset<unsigned char> &t, const std::string name) {
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

std::shared_ptr<SealibVisual::TikzPicture>
SealibVisual::TikzGenerator::generateTikzElement(
    const Sealib::Bitset<unsigned char> &t) {
  return generateTikzElement(t, "bitset");
}

std::shared_ptr<SealibVisual::TikzGraph>
SealibVisual::TikzGenerator::generateTikzElement(const Sealib::basicgraph_t &t,
                                                 bool directed) {
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
        if (directed) {
          graph->addEdge(key, "->");
        } else {
          graph->addEdge(key);
        }
      }
    }
  }

  return graph;
}

using Sealib::CompactArray;
using SealibVisual::TikzElement;
using SealibVisual::TikzPicture;
std::shared_ptr<TikzPicture> TikzGenerator::generateTikzElement(
    CompactArray &c, std::string name, std::string positionOpts) {
  std::vector<std::string> u;
  for (unsigned a = 0; a < c.size(); a++) {
    u.push_back(std::to_string(c.get(a)));
  }
  std::shared_ptr<TikzElement> array(new TikzArray(u, "C", "array", true));
  std::shared_ptr<TikzNode> label(new TikzNode("", "below=0.1cm of C", name));

  std::stringstream opts;
  if (positionOpts != "") opts << positionOpts << ",";
  opts << "array/.style={"
          "matrix of nodes,"
          "ampersand replacement=\\&,"
          "nodes={draw, minimum size=7mm, fill=white},"
          "column sep=-\\pgflinewidth, row sep=0.5mm, nodes in empty cells,"
          "row 1/.style={nodes={draw=none, fill=none, minimum size=5mm}}}";
  std::shared_ptr<TikzPicture> va(new TikzPicture(opts.str()));
  va->add(array);
  va->add(label);

  // std::shared_ptr<TikzPicture> vb(new TikzPicture("node distance=1mm"));
  // std::shared_ptr<TikzNode> node(
  //    new TikzNode("C", "anchor=north", va->toString()));
  // vb->add(node);
  // if (name != "") {
  //  std::shared_ptr<TikzNode> label(
  //      new TikzNode("C_label", "left=of C, yshift=-3mm", name));
  //  vb->add(label);
  //}
  // return vb;
  return va;
}

using SealibVisual::TikzStack;
std::shared_ptr<TikzPicture> TikzGenerator::generateTikzElement(
    std::vector<unsigned int> &v, std::string name, bool vertical,
    std::string positionOpts) {
  std::vector<std::string> s;
  for (unsigned a : v) {
    s.push_back(std::to_string(a));
  }
  std::shared_ptr<TikzElement> array;
  if (vertical) {
    array = std::shared_ptr<TikzElement>(
        new TikzStack(s, "S", "array,anchor=south", true));
  } else {
    array = std::shared_ptr<TikzElement>(new TikzArray(s, "S", "array", true));
  }
  std::shared_ptr<TikzNode> label =
      std::make_shared<TikzNode>("", "below=0.1cm of S", name);

  std::stringstream arrayStyle;
  if (positionOpts != "") arrayStyle << positionOpts << ",";
  arrayStyle
      << "array/.style={"
         "matrix of nodes,"
         "ampersand replacement=\\&,"
         "nodes={draw, minimum size=7mm, fill=white},"
         "column sep=-\\pgflinewidth, row sep=0.5mm, nodes in empty cells,";
  if (vertical) {
    arrayStyle
        << "column 1/.style={nodes={draw=none, fill=none, minimum size=5mm}}}";
  } else {
    arrayStyle
        << "row 1/.style={nodes={draw=none, fill=none, minimum size=5mm}}}";
  }

  std::shared_ptr<TikzPicture> va(new TikzPicture(arrayStyle.str()));
  va->add(array);
  va->add(label);

  // std::shared_ptr<TikzPicture> vb(new TikzPicture("node distance=1mm"));
  // std::shared_ptr<TikzNode> node(
  //    new TikzNode("V", "anchor=north", va->toString()));
  // vb->add(node);
  // if (name != "") {
  //  std::stringstream labelStyle;
  //  labelStyle << "left=of V,";
  //  if (vertical)
  //    labelStyle << "yshift=1mm";
  //  else
  //    labelStyle << "yshift=-3mm";
  //  std::shared_ptr<TikzNode> label(
  //      new TikzNode("V_label", labelStyle.str(), name));
  //  vb->add(label);
  //}
  // return vb;
  return va;
}

}  // namespace SealibVisual
