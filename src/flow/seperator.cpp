#include <sealib/graph/directedgraph.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/iterator/dfs.h>
#include <sealib/graph/node.h>
#include <sealib/flow/seperator.h>
#include "stgraph.h"

namespace Sealib {

std::vector<std::pair<uint64_t, uint64_t>> Seperator::e_seperate(Sealib::Bitset<> s,
        Sealib::Bitset<> t, Sealib::Graph const &g, int64_t k,
        Iterator<UserCall> *iter(Graph const &, uint64_t)){

    STGraph graph = Sealib::STGraph(g, s, t);

    // compute a residental network
    bool ispath = true;
    while (ispath && k >= 0) {
        k--;
        ispath = false;
        std::vector<uint64_t> path(0);
        Iterator<UserCall> *it = iter(graph, graph.getOrder() - 1);
        UserCall x = it->next();
        while((x.type != UserCall::postexplore || x.u != graph.getOrder() - 1) && !ispath){
            switch(x.type){
                case UserCall::preexplore:
                    path.push_back(x.u);
                    if(x.u > t.size()){
                        if (t.get(x.u)) ispath = true;
                    }
                    break;
                case UserCall::postexplore:
                    path.pop_back();
                    break;
            }
            x = it->next();
        }
        free(it);
        if(ispath) graph.revertpath(path);
    }
    if(k < 0) throw "no seperator with max k edges"; // interrupt

    // compute a set of edges in the minimum cut
    std::vector<std::pair<uint64_t, uint64_t>> es;
    Sealib::Bitset<> s_reachable = Sealib::Bitset<uint64_t>(graph.getOrder());
    Iterator<UserCall> *it = iter(graph, graph.getOrder() - 1);
    UserCall x = it->next();
    while(x.type != UserCall::postexplore || x.u != graph.getOrder() - 1){
        switch(x.type){
            case UserCall::preexplore:
                s_reachable[x.u] = 1;
                break;
        }
        x = it->next();
    }
    for (uint64_t i = 0; i < g.getOrder(); i++) {
        for (uint64_t j = 0; j < g.deg(i); j++) {
            if (s_reachable.get(i) == !s_reachable.get(g.head(i, j))) {
                es.push_back({i, g.head(i, j)});
            }
        }
    }
    return es;
}

std::vector<std::pair<uint64_t, uint64_t>> Seperator::e_seperate(Sealib::Bitset<> s,
        Sealib::Bitset<> t, Sealib::Graph const &g, int64_t k) {
    try{return Seperator::e_seperate(s, t, g, k, DFS::getStandardDFSIterator);}
    catch(std::string e){throw e;}
}

Sealib::Bitset<> Seperator::v_seperate(Sealib::Bitset<> s, Sealib::Bitset<> t,
        Sealib::Graph const &g, int64_t k,
        Iterator<UserCall> *iter(Graph const &, uint64_t)){
    // create a directed graph g' with in- and output vertices from g
    uint64_t s_deg = 0;
    int64_t snode = -1;
    Sealib::Bitset<> s_reach = Sealib::Bitset<uint64_t>(g.getOrder());
    std::vector<SimpleNode> nodes(g.getOrder() * 2);

    for (uint64_t i = 0; i < g.getOrder(); i++) {
        if (s.get(i)) { // sum up the vertices in s
            if (snode == -1) snode = i + g.getOrder();
            for (uint64_t j = 0; j < g.deg(i); j++) {
                // no double edges and no edges to vertices in s
                if(!s_reach.get(g.head(i, j)) && !s.get(g.head(i, j))){
                    s_deg++;
                    s_reach[g.head(i, j)] = 1;
                }
            }
        } else if (t.get(i)) {
            // Edges in t aren't important, because we won't use edges wich are
            // leading away from t. After we reached a vertice in t we stop.
        } else {
            std::vector<uint64_t> out(g.deg(i));
            for (uint64_t j = 0; j < g.deg(i); j++)
                out[j] = g.head(i, j);
            nodes[i + g.getOrder()] = SimpleNode(out);
        }
        std::vector<uint64_t> in(1);
        in[0] = i + g.getOrder();
        nodes[i] = SimpleNode(in);
    }

    std::vector<uint64_t> s_out(s_deg);
    for(uint64_t i = 0, s_pos = 0; i < g.getOrder(); i++){
        if(s_reach.get(i)){
            s_out[s_pos] = i;
            s_pos++;
        }
    }
    nodes[snode] = SimpleNode(s_out);
    Graph* graph = new Sealib::DirectedGraph(nodes);

    // compute a minimum edge seperator
    std::vector<std::pair<uint64_t, uint64_t>> es;
    Sealib::Bitset<> s2 = Sealib::Bitset<uint64_t>(s.size()*2);
    Sealib::Bitset<> t2 = Sealib::Bitset<uint64_t>(t.size()*2);
    for(uint64_t i = 0; i < s.size(); i++){
        s2[i] = s[i];
        s2[s.size()+i] = s[i];
        t2[i] = t[i];
        t2[t.size()+i] = t[i];
    }
    try {es = Seperator::e_seperate(s2, t2, *graph, k, iter);}
    catch (std::string e){
        if(e.compare("no seperator with max k edges") == 0) {
            throw "no seperator with max k vertices";
        }
        else throw e;
    }

    // get the minimum vertice seperator
    Sealib::Bitset<> vs = Sealib::Bitset<uint64_t>(g.getOrder());
    for (uint64_t i = 0; i < es.size(); i++){
        if(!(s2[es[i].first] || s2[es[i].second])) {
            if (es[i].first < vs.size()) { vs[es[i].first] = 1; }
            else { vs[es[i].second] = 1; }
        }
        else{
            if (es[i].first < vs.size()) { vs[es[i].second - g.getOrder()] = 1; }
            else { vs[es[i].first - g.getOrder()] = 1; }
        }
    }
    return vs;
}

Sealib::Bitset<> Seperator::v_seperate(Sealib::Bitset<> s, Sealib::Bitset<> t,
        Sealib::Graph const &g, int64_t k) {
    try {return Seperator::v_seperate(s, t, g, k, DFS::getStandardDFSIterator);}
    catch(std::string e){throw e;}
}

}