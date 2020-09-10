#ifndef SRC_VIEW_TEST_ALGORITHMS_H_
#define SRC_VIEW_TEST_ALGORITHMS_H_
#include <sealib/_types.h>
#include <future>

namespace Sealib {
/**
 * Helper class that contains prepared algorithm
 * runtime/space tests.
 */
class AlgorithmComparison {
 public:
    /**
     * Launch a selected algorithm and measure its time or space.
     * @param program string indicating the algorithm to launch
     * @param file1 output CSV file for the standard version
     * @param file2 output CSV file for the space-efficient version
     * @param from minimum order of which a graph is generated
     * @param to maximum order of which a graph is generated
     */
    static int launch(std::string program, const std::string& file1, const std::string& file2,
                      uint64_t from = 1e5, uint64_t to = 1e6);

    /**
     * Measure the runtime of two algorithms.
     * @param f1 function which runs an algorithm
     * @param f2 function which runs an algorithm
     * @param fg function generating a graph from a
     * given order
     */
    template <class F1, class F2, class G>
    static void measureTime(F1 f1, F2 f2, std::string file1, std::string file2,
                            G fg, uint64_t from = 1e5, uint64_t to = 1e6);

    template <class F, class G>
    static void measureFunction(F f, G g, std::promise<double> r);

    /**
     * Measure the memory consumption of two algorithms.
     * @param f1 function returning used memory in bytes
     * @param f2 function returning used memory in bytes
     * @param fg function generating a graph from a given order
     */
    template <class F1, class F2, class G>
    static void measureSpace(F1 f1, F2 f2, const std::string& file1, const std::string& file2,
                             G fg, uint64_t from = 1e5, uint64_t to = 1e6);

    template <class F, class G>
    static void evalFunction(F f, G g, std::promise<double> r);
};
}  // namespace Sealib
#endif  // SRC_VIEW_TEST_ALGORITHMS_H_
