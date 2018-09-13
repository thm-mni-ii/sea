#include <limits>
#include <sealib/graphcreator.h>

Sealib::Basicgraph Sealib::GraphCreator::createGraphFromAdjacencyMatrix(unsigned int **adjMatrix,
                                                                                         unsigned int order) {
    std::vector<Node> nodes;

  for (unsigned int i = 0; i < order; i++) {
    unsigned int deg = 0;

    for (unsigned int j = 0; j < order; j++) {
      deg += adjMatrix[i][j];
    }

        std::vector<Adjacency> adj(deg);

        int idx = 0;
        for (unsigned int j = 0; j < order; j++) {
            for (unsigned int k = 0; k < adjMatrix[i][j]; k++) {
                adj[idx++] = Adjacency(j);
            }
        }
        nodes.emplace_back(adj);
    }
    nodes[i] = Node(adj, deg);
  }

    for (unsigned int i = 0; i < order; i++) {
        const unsigned int deg = nodes[i].getDegree();
        const std::vector<Adjacency> &adj_arr = nodes[i].getAdj();

        for (unsigned int j = 0; j < deg; j++) {
            if (adj_arr[j].crossIndex == std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr[j].vertex;
                const std::vector<Adjacency> &_adj_arr = nodes[v].getAdj();
                const unsigned int _deg = nodes[v].getDegree();

                for (unsigned int _j = 0; _j < _deg; _j++) {
                    if (_adj_arr[_j].crossIndex == std::numeric_limits<unsigned int>::max() &&
                        _adj_arr[_j].vertex == i) {
                        nodes[v].setCrossIndex(_j, j);
                        nodes[i].setCrossIndex(j, _j);
                        break;
                    }
                }
            }
        }
      }
    }
  }

  return new Basicgraph(nodes, order);
}

static std::random_device rng;

Basicgraph *GraphCreator::createRandomFixed(unsigned int order,
                                            unsigned int degreePerNode) {
  std::uniform_int_distribution<unsigned int> rnd(0, order - 1);
  Node *n = reinterpret_cast<Node *>(malloc(sizeof(Node) * order));
  for (unsigned int a = 0; a < order; a++) {
    unsigned int ai = degreePerNode;
    Adjacency *ad =
        reinterpret_cast<Adjacency *>(malloc(sizeof(Adjacency) * ai));
    for (unsigned int b = 0; b < ai; b++) {
      ad[b] = Adjacency(rnd(rng));
    }
    n[a] = Node(ad, ai);
  }
  return new Basicgraph(n, order);
}

    return Basicgraph(nodes);
}
