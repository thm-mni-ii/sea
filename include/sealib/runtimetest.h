#ifndef SEALIB_RUNTIMETEST_H_
#define SEALIB_RUNTIMETEST_H_
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <tuple>
#include <vector>

/**
 * This class is used to perform runtime tests on
 * graph search algorithms. The results can either
 * be printed into the standard output or into a
 * CSV (comma seperated values) file.
 */
class RuntimeTest {
 private:
    std::vector<std::tuple<uint32_t, uint32_t>> parameters;
    std::vector<uint32_t> runtimes;

 public:
    /**
     * Measures the runtime of the given function (in milliseconds) and saves
     * the results.
     * @param testfunction The function that should be tested
     * @param order Order of the input graph
     * @param size Number of edges of the input graph
     */
    void runTest(std::function<void(void)> testfunction, uint32_t order,
                 uint32_t size);

    /**
     * Adds a result to the list of test results.
     * @param order Order of the input graph
     * @param size Number of edges of the input graph
     * @param result The result (runtime/space usage/...) to store
     */
    void addLine(uint32_t order, uint32_t size, uint64_t result);

    /**
     * Prints the test results to standard output.
     */
    void printResults();
    /**
     * Saves the test results to a CSV file.
     * @params filepath path to the output file (will be truncated)
     */
    void saveCSV(std::string filepath);
};

void RuntimeTest::runTest(std::function<void(void)> testfunction,
                          uint32_t order, uint32_t size) {
    using clk = std::chrono::high_resolution_clock;

    std::tuple<uint32_t, uint32_t> funcParameters(order, size);
    parameters.push_back(funcParameters);
    const clk::time_point runStart = clk::now();
    std::cout << "Running test: " << parameters.size() << " n: " << order
              << " w: " << size << std::endl;
    testfunction();
    const auto runTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                             clk::now() - runStart)
                             .count();

    runtimes.push_back(runTime);
}

void RuntimeTest::addLine(uint32_t order, uint32_t size, uint64_t result) {
    parameters.push_back({order, size});
    runtimes.push_back(result);
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
    output.open(filepath,
                std::ofstream::in | std::ofstream::out | std::ofstream::trunc);
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
