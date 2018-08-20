#include <memory>
#include "sealib/compactgraph.h"

using Sealib::Compactgraph;

Compactgraph::Compactgraph(unsigned int _A[]) {
	this->A = std::unique_ptr<unsigned int[]>(_A);
}

unsigned int Compactgraph::getNodeDegree(unsigned int u) {
	// Fix index u = u+1
	++u;
	// Following node
	if (A[u] == u) {
		return 0;
	}
	unsigned int v = u + 1;
	if (u != getOrder()) {
		unsigned int skip = 0;
		while (A[v+skip] <= getOrder() && v+skip <= getOrder()) {
			++skip;
		}
		v += skip;
		if (v > getOrder()) {
			return (A[0] + A[A[0]+1] + 2) - A[u];
		}
		return A[v] - A[u];
	} else {
		return (A[0] + A[A[0]+1] + 2) - A[u];
	}
}

unsigned int Compactgraph::head(unsigned int u, unsigned int k) {
	return A[A[u+1]+k];
}

unsigned int Compactgraph::getOrder() {
	return A[0];
}

unsigned int* Compactgraph::getData() {
	return this->A.get();
}
