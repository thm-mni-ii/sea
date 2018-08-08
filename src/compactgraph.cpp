#include <memory>
#include "sealib/compactgraph.h"

using Sealib::Compactgraph;

Compactgraph::Compactgraph(unsigned int _A[]){
	this->A = std::unique_ptr<unsigned int[]>(_A);
}

unsigned int Compactgraph::getNodeDegree(unsigned int u){
	if(u != A[0]){
		return A[u+1] - A[u];
	}else{
		return (A[0] + A[A[0]+1] + 2) - A[u];
	}
}

unsigned int Compactgraph::head(unsigned int u, unsigned int k){
	return A[A[u]+k];
}

unsigned int Compactgraph::getOrder(){
	return A[0]; 
} 
