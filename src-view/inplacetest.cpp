#include <iostream>
#include <ctime>
#include "sealib/graphrepresentations.h"
#include "sealib/graph.h"
#include "sealib/graphcreator.h"
#include "sealib/dfs.h"
#include "sealib/runtimemeasurer.h"

int main(){
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
  unsigned int n;
	unsigned int w;
	RuntimeMeasurer runner;
	n = 6000;
	w = n*(n-1) * 0.5;
	runner.runFunction([n,w](){Graphrepresentations::generateStandardGraph(n,0.5f);},n,w);
	n = 5000;
	w = n*(n-1) * 0.5;
	runner.runFunction([n,w](){Graphrepresentations::generateStandardGraph(n,0.5f);},n,w);
	n = 4000;
	w = n*(n-1) * 0.5;
	runner.runFunction([n,w](){Graphrepresentations::generateStandardGraph(n,0.5f);},n,w);
	n = 3000;
	w = n*(n-1) * 0.5;
	runner.runFunction([n,w](){Graphrepresentations::generateStandardGraph(n,0.5f);},n,w);
	n = 2000;
	w = n*(n-1) * 0.5;
	runner.runFunction([n,w](){Graphrepresentations::generateStandardGraph(n,0.5f);},n,w);
	n = 1000;
	w = n*(n-1) * 0.5;
	runner.runFunction([n,w](){Graphrepresentations::generateStandardGraph(n,0.5f);},n,w);
	runner.printResults();
	return 0;
}

