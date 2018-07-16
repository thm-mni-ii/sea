#include "sealib/node.h"

using Sealib::Adjacency;
using Sealib::Node;

Adjacency *Node::getAdj() { return adj; }

unsigned int Node::getDegree() { return deg; }

Node::Node(Adjacency *_adj, unsigned int _deg) : adj(_adj), deg(_deg) {}
