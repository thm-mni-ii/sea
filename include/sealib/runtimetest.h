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
    std::vector<std::tuple<uint64_t, uint64_t>> parameters;
    std::vector<double> runtimes;

 public:
    /**
     * Measures the runtime of the given function (in seconds) and saves
     * the results.
     * @param testfunction The function that should be tested
     * @param order Order of the input graph
     * @param size Number of edges of the input graph
     */
    void runTest(const std::function<void(void)>& testfunction, uint64_t order,
                 uint64_t size);

    /**
     * Adds a result to the list of test results.
     * @param order Order of the input graph
     * @param size Number of edges of the input graph
     * @param result The result (runtime/space usage/...) to store
     */
    void addLine(uint64_t order, uint64_t size, uint64_t result);
    void addLine(uint64_t order, uint64_t size, double result);

    /**
     * Prints the test results to standard output.
     */
    void printResults();

    /**
     * Saves the test results to a CSV file.
     * @params filepath path to the output file (will be truncated)
     */
    void saveCSV(const std::string& filepath,
                 const std::string& title = "order,size,runtime");
};

void RuntimeTest::runTest(const std::function<void(void)>& testfunction,
                          uint64_t order, uint64_t size) {
    struct tms t1{}, t2{};
    parameters.emplace_back(order, size);
    std::cout << "Running test: " << parameters.size() << " n: " << order
              << " m: " << size << std::endl;

    times(&t1);
    testfunction();
    times(&t2);

    clock_t start = t1.tms_utime + t1.tms_stime,
            end = t2.tms_utime + t2.tms_stime;
    double runTime = static_cast<double>(end - start) /
                     static_cast<double>(sysconf(_SC_CLK_TCK));
    runtimes.push_back(runTime);
}

void RuntimeTest::addLine(uint64_t order, uint64_t size, uint64_t result) {
    addLine(order, size, static_cast<double>(result));
}
void RuntimeTest::addLine(uint64_t order, uint64_t size, double result) {
    std::cout << "Adding result: " << parameters.size() << " n: " << order
              << " m: " << size << std::endl;
    parameters.emplace_back(order, size);
    runtimes.push_back(result);
}

void RuntimeTest::printResults() {
    std::cout << "order,size,result" << std::endl;
    for (uint64_t i = 0; i < parameters.size(); ++i) {
        std::cout << std::get<0>(parameters[i]) << ",";
        std::cout << std::get<1>(parameters[i]) << ",";
        std::cout << runtimes[i] << std::endl;
    }
}

void RuntimeTest::saveCSV(const std::string& filepath, const std::string& title) {
    std::ofstream output;
    output.open(filepath,
                std::ofstream::in | std::ofstream::out | std::ofstream::trunc);
    if (output.is_open()) {
        output << title << std::endl;
        for (uint64_t i = 0; i < parameters.size(); ++i) {
            output << std::get<0>(parameters[i]) << ",";
            output << std::get<1>(parameters[i]) << ",";
            output << runtimes[i];
            output << std::endl;
        }
    }
}

#endif  // SEALIB_RUNTIMETEST_H_
