#include "sealib/node.h"

Sealib::Adjacency *Sealib::Node::getAdj() { return adj; }

unsigned int Sealib::Node::getDegree() { return deg; }

Sealib::Node::Node(Sealib::Adjacency *_adj, unsigned int _deg) : adj(_adj), deg(_deg) {}
