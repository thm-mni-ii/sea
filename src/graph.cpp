#include "sealib/graph.h"

Node *Sealib::Graph::getNode(unsigned int u) { return &nodes[u]; }

unsigned int Sealib::Graph::head(unsigned int u, unsigned int k) { return nodes[u].getAdj()[k].vertex; }

unsigned int Sealib::Graph::getOrder() { return order; }

Sealib::Graph::Graph(Node *_nodes, unsigned int _order) : nodes(_nodes), order(_order) {}
