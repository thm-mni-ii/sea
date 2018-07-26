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
  unsigned int n = 50;
  float p = 0.5;	
  unsigned int error = 0;
  for(unsigned int i = 0; i < 1000; ++i){
	 unsigned int *a = Graphrepresentations::generateStandardGraph(n,p);
  	 unsigned int size = a[0] + a[a[0] + 1] + 2;
  	 unsigned int *b = new unsigned int[size];
		for(unsigned int j = 0; j < size; j++){
		  b[j] = a[j];
		}
	//std::cout << "standard representation:" << std::endl;
	//printGraph(a);
	Graphrepresentations::standardToBeginpointer(a);
	//std::cout << "beginpointer representation:" << std::endl;
	//printGraph(a);
	Graphrepresentations::swapRepresentation(a);
	//std::cout << "swapped beginpointer representation:" << std::endl;
	//printGraph(a);
	Graphrepresentations::swappedBeginpointerToStandard(a);
	//std::cout << "a standard representation:" << std::endl;
	//printGraph(a);
   bool equal = true;
   for(unsigned int j = 0; j < size; j++){
	  if(b[j] != a[j]){
		 equal = false;
		 }
	}
	if(!equal){
//	std::cout << "b standard representation:" << std::endl;
//	printGraph(b);
//	std::cout << std::endl;
	  error++;
	}
  delete a;
  delete b;
  }
  std::cout << error << std::endl;
	return 0;
}

