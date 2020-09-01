#include <sealib/graph/graphcreator.h>
#include "trackingallocator.hpp"
#include "cmdoptions.hpp"
#include <chrono>
#include "io.hpp"
#include <iostream>
#include <sealib/collection/subgraphstack.h>
#include <sealib/collection/simplesubgraphstack.hpp>


struct Measurement {
    uint64_t duration, memory;
};

template <class Stack>
Measurement measureSubgraphStack(
    const std::shared_ptr<Sealib::UndirectedGraph>& graph) {
    typedef Sealib::Bitset<uint64_t> bitset_t;

    Stack stack(graph);
    auto before_t = std::chrono::high_resolution_clock::now();
    auto before_m = s_alloc_metrics.current();

    for (uint64_t i = 0; i < 5; i++) {
        bitset_t a(stack.order(i));
        for (uint64_t j = 0; j < a.size(); j++) {
            a[j] = j % 4 != 0;
        }
        stack.push_vertex_induced(a);
    }

    auto after_t = std::chrono::high_resolution_clock::now();
    auto after_m = s_alloc_metrics.current();

    Measurement m{};
    m.memory = after_m - before_m;
    m.duration = std::chrono::duration_cast<std::chrono::milliseconds>(after_t -
                                                                       before_t)
                     .count();
    return m;
}

int main(int argc, char* argv[]) {
    if (opt_exists(argv, argv + argc, "-g")) {
        // Graph generation mode
        auto tmp = get_opt(argv, argv + argc, "-n");
        if (tmp == nullptr) {
            print_help(argv[0]);
            return 1;
        }
        auto n = std::stoul(tmp);

        tmp = get_opt(argv, argv + argc, "-s");
        uint64_t s;
        if (tmp == nullptr) {
            s = 0;
        } else {
            s = std::stoul(tmp);
        }
        tmp = get_opt(argv, argv + argc, "-p");
        if(tmp == nullptr) {
            print_help(argv[0]);
            return 1;
        }
        auto p = std::stod(tmp);

        tmp = get_opt(argv, argv + argc, "-f");
        if(tmp == nullptr) {
            print_help(argv[0]);
            return 1;
        }
        std::string path(tmp);

        std::cout << "Generating graph with n: "
                  << n << " p: " << p << " s: " << s << std::endl;
        std::shared_ptr<Sealib::UndirectedGraph> g =
            Sealib::GraphCreator::randomUndirected(n, p, s);
        std::cout << "Done! Writing to file " << path << " " << std::endl;
        write(g, path);
    } else {
        auto tmp = get_opt(argv, argv + argc, "-f");
        if(tmp == nullptr) {
            print_help(argv[0]);
            return 1;
        }
        std::string path(tmp);

        std::shared_ptr<Sealib::UndirectedGraph> graph = read(path);
        std::cout << "Graph with " << graph->getOrder() << " vertices" << std::endl;
        auto results = measureSubgraphStack<Sealib::SubGraphStack>(graph);
        auto simple_results =
            measureSubgraphStack<Sealib::SimpleSubGraphStack>(graph);
        std::cout << "SubGraphStack" << std::endl
                  << "Time: " << results.duration << " ms"
                  << " Memory: " << results.memory << std::endl;
        std::cout << "SimpleSubGraphStack" << std::endl
                  << "Time: " << simple_results.duration << " ms"
                  << " Memory: " << simple_results.memory << std::endl;
    }
}
