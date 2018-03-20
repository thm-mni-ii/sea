//
// Created by jmeintrup on 13.03.18.
//
#include "sealib/node.h"

/**
 * Getter for the adjacency array. Returns a pointer to the first element of the array.
 * @return pointer to adj
 */
Adjacency* Node::getAdj() { return adj; }

/**
 * Getter for the degree of the node.
 * @return value of deg
 */
size_t Node::getDeg() { return deg; }

/**
 * Constructor for the Node class.
 * @param _adj array of adjacencys of this node.
 * @param _deg degree of this node.
*/
Node::Node(Adjacency *_adj, size_t _deg) : adj(_adj),deg(_deg) {}
