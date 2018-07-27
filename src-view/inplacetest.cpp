#include <iostream>
#include <ctime>
#include "sealib/graphrepresentations.h"
#include "sealib/graph.h"
#include "sealib/graphcreator.h"
#include "sealib/dfs.h"

void printGraph(unsigned int* a){
	for(uint i = 0; i < a[0] + a[a[0]+1] + 2; ++i){
		std::cout << i << "\t";
	}
	std::cout << std::endl;
	for(uint i = 0; i < a[0] + a[a[0]+1] + 2; ++i){
		std::cout << a[i] << "\t";
	}
	std::cout << std::endl;
}
int main(){
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
  unsigned int n = 30000;
  float p = 0.5;	
	 unsigned int *a = Graphrepresentations::generateStandardGraph(n,p);
	 std::cout << "standard representation:" << std::endl;
	 printGraph(a);
	 Graphrepresentations::standardToBeginpointer(a);
	 Graphrepresentations::swapRepresentation(a);
	 printGraph(a);
	 DFS::runLinearTimeInplaceDFS(a,3);
  delete a;
	return 0;
}

