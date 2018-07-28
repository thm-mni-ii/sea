#include <iostream>
#include <random>
#include <ctime>
#include <unistd.h>
#include <cmath>
#include "sealib/graphrepresentations.h"
#include "sealib/graph.h"
#include "sealib/graphcreator.h"
#include "sealib/dfs.h"
#include "sealib/runtimemeasurer.h"

int main(){
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	unsigned int n[4] = {1000,3000,10000,30000};
	double p[16];
	for(unsigned int i = 0; i < 4; ++i){
		p[i*4] = 0.001;
		p[i*4+1] = 0.002;
		p[i*4+2] =	0.01; 
		p[i*4+3] =  0.02; 
	}
	std::default_random_engine gen(std::time(0));
	RuntimeMeasurer runner;
	for(unsigned int i = 0; i < 4; ++i){
		for(unsigned int j = 0; j < 4; ++j){
			for(unsigned int k = 0; k< 10; ++k){
				unsigned int* a = Graphrepresentations::generateStandardGraph(n[i],p[i*4+j],&gen);
				//w = num edges
				unsigned int w = a[a[0]+1];
				Graph* g = Graphrepresentations::standardToGraph(a);
				delete a;
				runner.runFunction([g](){DFS::standardDFS(g,nullptr,nullptr,nullptr,nullptr);},n[i],w);
			}
		}
	}
	runner.printToCSV("/home/simon/runtimeresults.csv");
	return 0;
}

