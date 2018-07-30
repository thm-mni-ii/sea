#include <sys/times.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>

class RuntimeMeasurer{
	private:
		std::vector<std::tuple<unsigned int,unsigned int> > parameters;
		//runtimes in seconds
		std::vector<double> runtimes;

	public:
		template<typename Func>
			void runFunction(Func funktor,unsigned int nodes, unsigned int edges);
		void printResults();	
		void printToCSV(std::string path);
};

template<typename Func>
void RuntimeMeasurer::runFunction(Func funktor, unsigned int nodes, unsigned int edges){
	std::tuple<unsigned int,unsigned int> funcParameters(nodes,edges);
	parameters.push_back(funcParameters);
	struct tms before;
	struct tms after; 
	std::cout << "Running test: "<< parameters.size() << " n: "<< nodes << " w: "<< edges << std::endl;
	times(&before);
	funktor();
	times(&after);
	double elapsedTime = ((double)((after.tms_utime + after.tms_stime)
			- (before.tms_utime + before.tms_stime))) / sysconf(_SC_CLK_TCK);
	runtimes.push_back(elapsedTime); 
} 

void RuntimeMeasurer::printResults(){
	for(unsigned int i = 0; i < parameters.size(); ++i){
		std::cout <<"Nodes: " << std::get<0>(parameters[i]) << std::endl;
		std::cout <<"Edges: " << std::get<1>(parameters[i]) << std::endl;	
		std::cout <<"Runtime: " << runtimes[i] << std::endl;
	}
}

void RuntimeMeasurer::printToCSV(std::string path){
	std::ofstream output;
	output.open(path,std::ofstream::out | std::ofstream::trunc);
	output << "order,size,runtime" << std::endl;
	for(unsigned int i = 0; i < parameters.size(); ++i){
		output<<std::get<0>(parameters[i])<<",";
		output<<std::get<1>(parameters[i])<<",";
		output<<runtimes[i];
		output<<std::endl;
	}
	output.close();
}
