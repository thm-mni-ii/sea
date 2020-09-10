#include <sealib/graph/undirectedgraph.h>
#include <memory>
#include <iostream>
#include <fstream>

void write(const std::shared_ptr<Sealib::UndirectedGraph>& graph, const std::string& path) {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    uint64_t order = graph->getOrder();
    file.write((char*) &order, sizeof(uint64_t));
    for(size_t i = 0; i < order; i++) {
        uint64_t degree = graph->getNode(i).getAdj().size();
        uint64_t size = sizeof(std::pair<uint64_t, uint64_t>) * degree;
        file.write((char*)&degree, sizeof(uint64_t));
        auto ptr = (char*) graph->getNode(i).getAdj().data();
        file.write(ptr, size);
    }
    file.close();
}

std::unique_ptr<Sealib::UndirectedGraph> read(const std::string& path) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path, std::ios::in | std::ios::binary);
    uint64_t order;
    file.read ((char*) &order, sizeof(uint64_t));
    auto g = std::make_unique<Sealib::UndirectedGraph>(order);
    for(size_t i = 0; i < order; i++) {
        uint64_t degree;
        file.read ((char*) &degree, sizeof(uint64_t));
        Sealib::ExtendedNode &node = g->getNode(i);
        node.getAdj().resize(degree);
        file.read((char*) node.getAdj().data(), sizeof(std::pair<uint64_t, uint64_t>) * degree);
    }
    return g;
}