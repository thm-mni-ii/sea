#include <sealib/graph.h>
#include <iostream>
#include "sealib/math.h"

using namespace math;
using namespace std;

int main() {
    int order = 4;               //1 2 3 4   1 2 3 4   1 2 3 4   1 2 3 4
    int **adj_mtrx = (int**) malloc(sizeof(int) * 4 * 4);
    adj_mtrx[0] = new int[order]{0,1,0,1};
    adj_mtrx[1] = new int[order]{1,0,1,0};
    adj_mtrx[2] = new int[order]{0,1,0,1};
    adj_mtrx[3] = new int[order]{1,0,1,0};

    Graph *g = new Graph(adj_mtrx, order);

    for(int i = 0; i < g->get_order(); i++) {
        
    }
}