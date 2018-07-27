#include <sys/times.h>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>

class RuntimeMeasurer{
	private:
		std::string path; 
		std::vector<std::tuple<unsigned int,unsigned int> > parameters;
		std::vector<unsigned int> runtimes;

	public:
		RuntimeMeasurer(std::string s);
		template<typename Func>
			void runFunction(Func funktor,unsigned int nodes, unsigned int edges);
		void printResults();	
};

RuntimeMeasurer(std::string s){
	path = s;
};

template<typename Func>
void RuntimeMeasurer::runFunction(Func funktor, unsigned int nodes, unsigned int edges){
	std::tuple<unsigned int,unsigned int> funcParameters(nodes,edges);
	parameters.push_back(funcParameters);
	struct tms before;
	struct tms after; 
	times(&before);
	funktor();
	times(&after);
	runtimes.push_back((after.tms_utime + after.tms_stime)
			- (before.tms_utime + before.tms_stime)); 
} 

void RuntimeMeasurer::printResults(){
	for(unsigned int i = 0; i < parameters.size(); ++i){
		std::cout <<"Nodes: " << std::get<0>(parameters[i]) << std::endl;
		std::cout <<"Edges: " << std::get<1>(parameters[i]) << std::endl;	
		std::cout <<"Runtime: " << runtimes[i] << std::endl;
	}
}

