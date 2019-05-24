#ifndef SEA_INOUTGRAPH_H
#define SEA_INOUTGRAPH_H

#include <sealib/graph/graph.h>

namespace Sealib {
/**
 * The class InOutGraph class is a wrapper so that the vseperator
 * funktion can provide the Graph for the eseperate funktion.
 * @author Vytautas Hermann
 */
class InOutGraph : public Graph {
public:
    explicit InOutGraph(Graph const &graph):
            g(graph){}
    uint64_t deg(uint64_t v)const override{
        if(v < g.getOrder()) return 1;
        else return g.deg(v - g.getOrder());
    }
    uint64_t head(uint64_t u, uint64_t k)const override{
        if(u < g.getOrder()) return u + g.getOrder();
        else return g.head(u - g.getOrder(), k);
    }
    uint64_t getOrder()const override{
        return g.getOrder() * 2;
    }
private:
    Graph const &g;
};

}

#endif //SEA_INOUTGRAPH_H
