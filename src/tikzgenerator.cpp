#include <sealib/tikzgenerator.h>
#include <sealib/bitset.h>
#include <sealib/tikzarray.h>
#include <sealib/tikznode.h>
#include <include/sealib/tikzdocument.h>
#include <include/sealib/tikzpicture.h>
#include <include/sealib/basicgraph.h>
#include <include/sealib/tikzgraph.h>
#include <tuple>

typedef Sealib::Bitset<unsigned char> bitset_t;
typedef Sealib::Basicgraph basicgraph_t;

template<>
std::shared_ptr<SealibVisual::TikzElement>
SealibVisual::TikzGenerator::generateTikzElement<bitset_t>(
    const bitset_t &t,
    const std::string name) {
    std::vector<std::string> arr(t.size());

    for (unsigned int i = 0; i < t.size(); i++) {
        arr[i] = t[i] ? "1" : "0";
    }
    std::shared_ptr<TikzPicture> inner(
        new TikzPicture("array/.style={"
                        "matrix of nodes,"
                        "ampersand replacement=\\&,"
                        "nodes={draw, minimum size=7mm, fill=green!30},"
                        "column sep=-\\pgflinewidth, row sep=0.5mm, nodes in empty cells,\n"
                        "\t\trow 1/.style={nodes={draw=none, fill=none, minimum size=5mm}}}"));

    std::shared_ptr<TikzElement> tikzArray(new TikzArray(arr, "array", "array", true));
    inner->add(tikzArray);

    std::shared_ptr<TikzPicture> outer(
        new TikzPicture());
    std::shared_ptr<TikzNode> node1(
        new TikzNode(name, "anchor=west", inner->toString()));
    outer->add(node1);
    std::shared_ptr<TikzNode> node2(
        new TikzNode("label", "anchor=west", name + ":", "(0.1,1)"));
    outer->add(node2);

    return outer;
}

template<>
std::shared_ptr<SealibVisual::TikzElement>
SealibVisual::TikzGenerator::generateTikzElement<bitset_t>(const bitset_t &t) {
    return generateTikzElement(t, "bitset");
}

template<>
std::shared_ptr<SealibVisual::TikzElement>
SealibVisual::TikzGenerator::generateTikzElement<basicgraph_t>(
    const basicgraph_t &t,
    const std::string name) {
    using std::string;
    using std::shared_ptr;
    using std::tuple;
    using std::make_tuple;
    using std::to_string;
    shared_ptr<SealibVisual::TikzGraph> graph(new SealibVisual::TikzGraph(t.getOrder()));

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
