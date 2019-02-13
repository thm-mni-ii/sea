#ifndef SEALIB_RUNTIMETEST_H_
#define SEALIB_RUNTIMETEST_H_
#include <sys/times.h>
#include <unistd.h>
#include <cstdint>
#include <ctime>
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
    std::vector<double> runtimes;

 public:
    /**
     * Measures the runtime of the given function (in seconds) and saves
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
    void saveCSV(std::string filepath,
                 std::string title = "order,size,runtime");
};

void RuntimeTest::runTest(std::function<void(void)> testfunction,
                          uint32_t order, uint32_t size) {
    struct tms t1, t2;
    parameters.push_back({order, size});
    std::cout << "Running test: " << parameters.size() << " n: " << order
              << " m: " << size << std::endl;

    times(&t1);
    testfunction();
    times(&t2);

    clock_t start = t1.tms_utime + t1.tms_stime,
            end = t2.tms_utime + t2.tms_stime;
    double runTime = (end - start) / (0. + sysconf(_SC_CLK_TCK));
    runtimes.push_back(runTime);
}

void RuntimeTest::addLine(uint32_t order, uint32_t size, uint64_t result) {
    std::cout << "Adding result: " << parameters.size() << " n: " << order
              << " m: " << size << std::endl;
    parameters.push_back({order, size});
    runtimes.push_back(result);
}

void RuntimeTest::printResults() {
    std::cout << "order,size,result" << std::endl;
    for (uint32_t i = 0; i < parameters.size(); ++i) {
        std::cout << std::get<0>(parameters[i]) << ",";
        std::cout << std::get<1>(parameters[i]) << ",";
        std::cout << runtimes[i] << std::endl;
    }
}

void RuntimeTest::saveCSV(std::string filepath, std::string title) {
    std::ofstream output;
    output.open(filepath,
                std::ofstream::in | std::ofstream::out | std::ofstream::trunc);
    if (output.is_open()) {
        output << title << std::endl;
        for (uint32_t i = 0; i < parameters.size(); ++i) {
            output << std::get<0>(parameters[i]) << ",";
            output << std::get<1>(parameters[i]) << ",";
            output << runtimes[i];
            output << std::endl;
        }
    }
}

#endif  // SEALIB_RUNTIMETEST_H_
