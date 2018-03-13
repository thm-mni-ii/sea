//
// Created by jmeintrup on 12.03.18.
//

#ifndef SEA_ADJACENCY_H
#define SEA_ADJACENCY_H
class Adjacency {
public:
    int vertex;
    int index;
    /**
     * Adjacency constructor.
     * Vertex is the number of the vertex
     * Index has to be set after the other vertex to be crosslinked with has been created
     * @param vertex
     */
    Adjacency(int vertex):vertex(vertex){
        index = -1;
    };
};
#endif //SEA_ADJACENCY_H
