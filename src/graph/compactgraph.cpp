#include <memory>
#include "sealib/graph/compactgraph.h"

using Sealib::CompactGraph;

CompactGraph::CompactGraph(uint32_t _A[]) {
	this->A = std::unique_ptr<uint32_t[]>(_A);
}

uint32_t CompactGraph::getNodeDegree(uint32_t u) const {
	// Fix index u = u+1
	++u;
	// Following node
	if (A[u] == u) {
		return 0;
	}
	uint32_t v = u + 1;
	if (u != getOrder()) {
		return A[v] - A[u];

		/*
		uint32_t skip = 0;
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

uint32_t CompactGraph::head(uint32_t u, uint32_t k) const {
	return A[A[u+1]+k];
}

uint32_t CompactGraph::getOrder() const {
	return A[0];
}

uint32_t* CompactGraph::getData() {
	return this->A.get();
}
