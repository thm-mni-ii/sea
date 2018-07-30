#include <sys/times.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
/**
 * This class is used to perform runtime tests on
 * graph search algorithms. The results can either
 * be printed into the standard output or into a 
 * CSV (comma seperated values) file.
 */
class RuntimeTest{
	private:
		std::vector<std::tuple<unsigned int,unsigned int> > parameters;
		std::vector<double> runtimes; ///<runtime in seconds
	public:
		/**
		 * Runs a test with a given function and saves the results
		 * @param testfunction a template type to be used with a lambda to specify
		 * wich functions runtime shall be tested
		 * @param order order of the tested graph used for running the test
		 * @param size size of the tested graph used for running the test
		 */
		template<typename Testfunction>
		void runTest(Testfunction testfunction,unsigned int order, unsigned int size);
		/**
		 * Prints the testresults to standard output
		 */
		void printResults();	
		/**
		 * Saves the testresults
		 * @params filepath path to the file in wich the results are to be saved
		 * the file will be truncated
		 */
		void saveCSV(std::string filepath);
};

template<typename Testfunction>
void RuntimeTest::runTest(Testfunction testfunction, unsigned int order, unsigned int size){
	std::tuple<unsigned int,unsigned int> funcParameters(order,size);
	parameters.push_back(funcParameters);
	struct tms before;
	struct tms after; 
	std::cout << "Running test: "<< parameters.size() << " n: "<< order << " w: "<< size << std::endl;
	times(&before);
	testfunction();
	times(&after);
	double elapsedTime = ((double)((after.tms_utime + after.tms_stime)
			- (before.tms_utime + before.tms_stime))) / sysconf(_SC_CLK_TCK);
	runtimes.push_back(elapsedTime); 
} 

void RuntimeTest::printResults(){
	for(unsigned int i = 0; i < parameters.size(); ++i){
		std::cout <<"Nodes: " << std::get<0>(parameters[i]) << std::endl;
		std::cout <<"Edges: " << std::get<1>(parameters[i]) << std::endl;	
		std::cout <<"Runtime: " << runtimes[i] << std::endl;
	}
}

void RuntimeTest::saveCSV(std::string filepath){
	std::ofstream output;
	output.open(filepath,std::ofstream::out | std::ofstream::trunc);
	output << "order,size,runtime" << std::endl;
	for(unsigned int i = 0; i < parameters.size(); ++i){
		output<<std::get<0>(parameters[i])<<",";
		output<<std::get<1>(parameters[i])<<",";
		output<<runtimes[i];
		output<<std::endl;
	}
	output.close();
}
