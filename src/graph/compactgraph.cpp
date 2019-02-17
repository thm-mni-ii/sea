#include <memory>
#include "sealib/graph/compactgraph.h"

using Sealib::CompactGraph;

CompactGraph::CompactGraph(uint64_t _A[]) {
	this->A = std::unique_ptr<uint64_t[]>(_A);
}

uint64_t CompactGraph::deg(uint64_t u) const {
	// Fix index u = u+1
	++u;
	// Following node
	if (A[u] == u) {
		return 0;
	}
	uint64_t v = u + 1;
	if (u != getOrder()) {
		return A[v] - A[u];

		/*
		uint64_t skip = 0;
		while (A[v + skip] <= getOrder() && (v + skip) <= getOrder()) {
			++skip;
		}
		v += skip;
		if (v > getOrder()) {
			return (A[0] + A[A[0]+1] + 2) - A[u];
		}
		return A[v] - A[u];
		*/
	} else {
		return (A[0] + A[A[0]+1] + 2) - A[u];
	}
}

uint64_t CompactGraph::head(uint64_t u, uint64_t k) const {
	return A[A[u+1]+k];
}

uint64_t CompactGraph::getOrder() const {
	return A[0];
}

uint64_t* CompactGraph::getData() {
	return this->A.get();
}
