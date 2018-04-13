#include "sealib/graph.h"

Node *Graph::getNode(unsigned int u) { return &nodes[u]; }

unsigned int Graph::head(unsigned int u, unsigned int k) { return nodes[u].getAdj()[k].vertex; }

unsigned int Graph::getOrder() { return order; }

Graph::Graph(Node *_nodes, unsigned int _order) : nodes(_nodes), order(_order) {}
