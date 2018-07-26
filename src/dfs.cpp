#include <stdio.h>
#include <iostream>
#include "sealib/dfs.h"

using Sealib::DFS;
using Sealib::Node;

// starting point of the DFS algorithm: O(n+m) time, O(n*log n) bits
void DFS::process_standard(Graph *g,
						   UserFunc1 preProcess, UserFunc2 preExplore,
						   UserFunc2 postExplore, UserFunc1 postProcess,
						   uint *color, uint u) {
	color[u] = DFS_GRAY;
	Node *un = g->getNode(u);
	if (preProcess != DFS_NOP_PROCESS) preProcess(un);
	for (uint k = 0; k < un->getDegree(); k++) {
		uint v = g->head(u, k);
		Node *vn = g->getNode(v);
		if (preExplore != DFS_NOP_EXPLORE) {
			preExplore(un, vn);
		}
		if (color[v] == DFS_WHITE) {
			process_standard(g, preProcess, preExplore, postExplore, postProcess, color, v);
		}
		if (postExplore != DFS_NOP_EXPLORE) {
			postExplore(un, vn);
		}
	}
	if (postProcess != DFS_NOP_PROCESS) {
		postProcess(un);
	}
	color[u] = DFS_BLACK;
}

void DFS::runStandardDFS(Graph *g, void (*preProcess)(Node *), void (*preExplore)(Node *, Node *),
						 void (*postExplore)(Node *, Node *), void (*postProcess)(Node *)) {
	uint *color = new uint[g->getOrder()];
	for (uint u = 0; u < g->getOrder(); u++) {
		color[u] = DFS_WHITE;
	}
	for (uint u=0; u < g->getOrder(); u++) {
		if (color[u] == DFS_WHITE) {
			process_standard(g, preProcess, preExplore, postExplore, postProcess, color, u);
		}
	}
	delete[] color;
}

class LinearTimeInplaceDFSRunner {
 private:
    unsigned int *A;
    unsigned int n;
    unsigned int N;
    unsigned int startVertex;
    unsigned int startPos;

    // only for debugging
    bool reachedEnd = false;

 public:
    LinearTimeInplaceDFSRunner(unsigned int *graph) {
        this->A = graph;
        this->n = A[0];
        this->N = A[n + 1] + n + 1;
        std::cout << "this->N = " << this->N << std::endl;
    }

    void run(unsigned int _startVertex) {
        this->startVertex = _startVertex;
        unsigned int p = n + 2;
        std::cout << "Start vertex: " << _startVertex << std::endl;
        std::cout << "Start with position: " << p << std::endl;
        while (A[p] != _startVertex && p <= N) {
            p = p + 1;
        }

        // TODO: if p > N => not found => panic
        this->startPos = p;
        visit(p);
    }

 private:
    unsigned int rA(unsigned int i) { if (A[i] == 0 || A[i] > n) return A[i]; else return A[A[i]]; }
    void wA(unsigned int i, unsigned int v) { if (A[i] == 0 || A[i] > n) A[i] = v; else A[A[i]] = v; }
    unsigned int name(unsigned int i) { if (A[i] <= n) return A[i]; else return 0u; }

    void printState() {
        for (unsigned int i = 0; i <= this->N; i++) {
            std::cout << this->A[i] << "\t";
            if (i == 0 || i == this->n + 1) {
                std::cout << " | ";
            }
        }
        std::cout << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t   ";
        for (unsigned int i = this->n + 2; i <= this->N; i++) {
            std::cout << rA(i) << "\t";
        }
        std::cout << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t   ";
        for (unsigned int i = this->n + 2; i <= this->N; i++) {
            std::cout << rA_(i) << "\t";
        }
        std::cout << std::endl;
    }

    unsigned int rA_(unsigned int i) {
        unsigned int Ai = rA(i);
        if (name(i) > 0 || name(Ai) > 0) {
            return Ai;
        } else if (name(Ai - 1) > 0) {
            return rA(Ai - 1);
        } else {
            return rA(Ai);
        }
    }

    void wA_(unsigned int i, unsigned int v) {
        unsigned int Ai = rA(i);
        if (name(i) > 0 || name(Ai) > 0) {
            return wA(i, v);
        } else if (name(Ai - 1) > 0) {
            return wA(Ai - 1, v);
        } else {
            return wA(Ai, v);
        }
    }

    bool isWhite(unsigned int p) {
        bool start = name(p) != this->startVertex;
        unsigned int p1 = A[p + 1]; // where do I point to

        std::cout << "isWhite(" << p << ") == " << start << " && " << name(p1) << " | A[" << 7 << " + 1] = " << p1 <<  std::endl;
        return name(p) != this->startVertex && name(A[p + 1]) != 0;
    }

    void visit(unsigned int p) {
        unsigned int v = name(p);
        if (v == 0) {
            std::cout << "visiting a position that does not store a vertex name" << std::endl;
            exit(1);
        }
        // TODO: if v == 0 => panic

        std::cout << "Visiting vertex: " << v << std::endl; // replace by preprocess
        printState();

        if (reachedEnd) {
            std::cout << "Reached end" << std::endl;
            //return;
        }

        nextNeighbor(p, true);
    };

    void nextNeighbor(unsigned int p, bool ignoreFirstCheck) {
        // First Check
        //std::cout << "enter nextNeighbor(" << p << ")" << std::endl;
        bool namedPos = name(p) != 0;
        if (p > this->N || (namedPos && !ignoreFirstCheck)) {  // Backtracking required
            if (p > this->N) {
                std::cout << "Reached end of memory" << std::endl;
            }

            unsigned int q = p - 1;
            while (name(q) == 0) {
                q = q - 1;
            }
            if (this->startPos == q) {
                std::cout << "end DFS!" << std::endl;
                printState();
                return;
            }
            std::cout << "goToParent(" << q << ") from first check" << std::endl;

            printState();

            if (p > this->N) {
                std::cout << "Reached end of memory" << std::endl;
                this->reachedEnd = true;
            }

            goToParent(q);
        } else {
            unsigned int rap = rA_(p);
            std::cout << "rap(" << rap << ")" << std::endl;
            if (isWhite(rap)) {
                std::cout << "goToChild(" << p << ")" << std::endl;
                goToChild(p);
            } else {
                if (namedPos && !isWhite(rA_(p))) {
                    if (!isWhite(rA_(p + 1))) {
                        nextNeighbor(p + 2, false);
                    } else {
                        unsigned int temp = rA_(p + 1);
                        wA_(p + 1, rA_(p));
                        wA_(p, temp);
                        std::cout << "swap(" << p + 1 << ", " << p << ")" << std::endl;
                        printState();

                        nextNeighbor(p, true);
                    }
                } else {
                    std::cout << "nextNeighbor(" <<  p + 1 << ", false)" << std::endl;
                    nextNeighbor(p + 1, false);
                }
            }
        }
    }

    void goToParent(unsigned int q) { // The root vertex does not get restored very well !!!
        std::cout << "goToParent(" << q << ")" << std::endl;
        unsigned int x = rA_(q + 1);
        unsigned int p = A[q + 1];

        std::cout << "(x = " << x << ", p = " << p << ")" << std::endl;

        wA(q + 1, x + 1); // Restore childs edge => Okay only if we have two vertices.

        if (name(p - 1) == 0) {
            wA(p, q); // Restore the pointer to the child
            nextNeighbor(p + 1, false);
        } else {
            wA(p - 1, q); // Restore the pointer to the child, because we never leave from the second position
            // we left over the first whenever p points to the second.

            if (rA_(p - 1) < rA_(p)) {
                nextNeighbor(p, false);
            } else {
                std::cout << "swap(" << (p - 1) << ", " << p << ")" << std::endl;
                printState();
                unsigned int temp = rA_(p - 1);
                std::cout << "- swap(" << temp << ", " << rA_(p) << ")" << std::endl;
                wA_(p - 1, rA_(p));
                wA_(p, temp);
                printState();
                nextNeighbor(p + 1, false);
            }
        }
    }

    void goToChild(unsigned int p) {
        unsigned int q = rA_(p); // First
        //unsigned int v = name(q); // Second

        // Is v discovered from the first position?
        bool fromFirstPosition = name(p) != 0;

        // Third: Create a pointer for later backtrack operation.
        wA(p, rA(q + 1));

        if (fromFirstPosition) {
            wA(q + 1, p + 1);
        } else {
            wA(q + 1, p);
        }

        visit(q);
    };
};

void DFS::runLinearTimeInplaceDFS(unsigned int *graph, unsigned int startVertex) {
    LinearTimeInplaceDFSRunner *ilDFSRunner = new LinearTimeInplaceDFSRunner(graph);
    ilDFSRunner->run(startVertex);
}
