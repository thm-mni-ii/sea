#include <sealib/naivetrail.h>
#include <set>
#include <map>

Sealib::NaiveTrail::NaiveTrail(const Sealib::NaiveTrail::graphptr_t &graph) {
    std::set<unsigned long> white;
    std::set<unsigned long> gray;
    std::set<unsigned long> black;
    std::set<unsigned long> even;
    std::set<unsigned long> uneven;

    std::vector<std::list<unsigned long>> g(graph->getOrder());

    for(unsigned int u = 0; u < graph->getOrder(); u++) {
        Sealib::Node &n = graph->getNode(u);
        if(n.getDegree() % 2 == 0) {  //even
            even.insert(u);
        } else {  // uneven
            even.insert(u);
        }

        if(n.getDegree() == 0) {
            black.insert(u);
        } else {
            white.insert(u);
        }
        for(auto &a : n.getAdj()) {
            g[u].push_back(a.vertex);
        }
    }

    std::list<unsigned long> trail;

    auto u = (unsigned long) - 1;
    if(!uneven.empty()) {
        u = (*uneven.begin());
        uneven.erase(u);
    } else if(!gray.empty()) {
        u = (*gray.begin());
        gray.erase(u);
    } else if(!white.empty()) {
        u = (*white.begin());
        white.erase(u);
    }
    while( u != (unsigned long) - 1) {
        auto kOld = {(unsigned long) - 1, (unsigned long) - 1};
        if (even.find(u) != even.end() && gray.find(u) != even.end()) {  // remember aOld
            for(unsigned int i = 0; i < trails.size(); i++) {
                unsigned long j = 0;
                for(const auto &a: trails[i]) {
                    if(a == u && j % 2 == 0 && j != t.size() - 1) {
                        kOld = {i, j};
                    }
                    j++;
                }
            }
        }
        

        trails.push_back(trail);
    }
}
