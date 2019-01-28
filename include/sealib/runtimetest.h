#ifndef SEALIB_RUNTIMETEST_H_
#define SEALIB_RUNTIMETEST_H_
#include <sys/times.h>
#include <unistd.h>
#include <cstdint>
#include <algorithm>
#include <chrono>
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
class RuntimeTest {
 private:
    std::vector<std::tuple<uint32_t, uint32_t>> parameters;
    std::vector<uint32_t> runtimes;  // Runtime in seconds
 public:
    /**
     * Runs a test with a given function and saves the results
     * @param testfunction a template type to be used with a lambda to specify
     * wich functions runtime shall be tested
     * @param order order of the tested graph used for running the test
     * @param size size of the tested graph used for running the test
     */
    template<typename Testfunction>
    void runTest(Testfunction testfunction, uint32_t order, uint32_t size);
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
void RuntimeTest::runTest(Testfunction testfunction, uint32_t order, uint32_t size) {
    using clk = std::chrono::high_resolution_clock;

    std::tuple<uint32_t, uint32_t> funcParameters(order, size);
    parameters.push_back(funcParameters);
    const clk::time_point runStart = clk::now();
    std::cout << "Running test: "<< parameters.size()
              << " n: "<< order << " w: "<< size << std::endl;
    testfunction();
    const auto runTime = std::chrono::duration_cast<std::chrono::milliseconds>
                         (clk::now() - runStart).count();

    runtimes.push_back(runTime);
}

void RuntimeTest::printResults() {
    std::cout << "order,size,runtime" << std::endl;
    for (uint32_t i = 0; i < parameters.size(); ++i) {
        std::cout << std::get<0>(parameters[i]) << ",";
        std::cout << std::get<1>(parameters[i]) << ",";
        std::cout << runtimes[i] << std::endl;
    }
}

void RuntimeTest::saveCSV(std::string filepath) {
    std::ofstream output;
    output.open(filepath, std::ofstream::in | std::ofstream::out | std::ofstream::trunc);
    if (output.is_open()) {
        output << "order,size,runtime" << std::endl;
        for (uint32_t i = 0; i < parameters.size(); ++i) {
            output << std::get<0>(parameters[i]) << ",";
            output << std::get<1>(parameters[i]) << ",";
            output << runtimes[i];
            output << std::endl;
        }
    }
}

#endif  // SEALIB_RUNTIMETEST_H_
