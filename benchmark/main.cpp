#include <sealib/graph/graphcreator.h>
#include "trackingallocator.hpp"
#include "cmdoptions.hpp"
#include "io.hpp"
#include <iostream>
#include <filesystem>
#include <sealib/collection/subgraphstack.h>
#include <sealib/collection/simplesubgraphstack.hpp>


struct Measurement {
    uint64_t duration, memory;
};

template<class Stack>
Measurement measureSubgraphStack(std::shared_ptr<Sealib::UndirectedGraph> graph) {
    typedef Sealib::Bitset<uint64_t> bitset_t;

    Stack stack(graph);
    std::chrono::high_resolution_clock clock;
    auto before_t = clock.now();
    auto before_m = s_alloc_metrics.current();

    for (uint64_t i = 0; i < 5; i++) {
        bitset_t a(stack.order(i));
        for (uint64_t j = 0; j < a.size(); j++) {
            if (j % 4 != 0) {
                a[j] = 1;
            }
        }
        stack.push_vertex_induced(a);
    }

    auto after_t = clock.now();
    auto after_m = s_alloc_metrics.current();

    Measurement m;
    m.memory = after_m - before_m;
    m.duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(after_t-before_t).count();
    return m;
}


int main(int argc, char * argv[]) {
    /*auto before = s_alloc_metrics.current();
    std::cout << "Allocated before graph: " << before << " bytes" << std::endl;
    std::shared_ptr<Sealib::UndirectedGraph> g =
        std::move(Sealib::GraphCreator::randomUndirected(100, 0.1));

    write(g, "dump.dat");

    auto h = read("dump.dat");

    auto after = s_alloc_metrics.current();
    std::cout << "Allocated for graph: " << after - before << " bytes" << std::endl;
    std::cout << "Total Current Allocated: " << after << " bytes" << std::endl;
    std::cout << "Total Overall Allocated: " << s_alloc_metrics.total_allocated << " bytes" << std::endl;*/

    if(opt_exists(argv, argv + argc, "-g"))
    {
        // Graph generation mode
        auto tmp = get_opt(argv, argv + argc, "-n");
        if(tmp == nullptr) {
            print_help(argv[0]);
            return 1;
        }
        auto n = std::stoul(tmp);

        tmp = get_opt(argv, argv + argc, "-s");
        uint64_t s;
        if(tmp == nullptr) {
            s = 0;
        }
        s = std::stoul(tmp);
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
        std::filesystem::path path(tmp);

        std::cout << "Generating graph with n: "
                  << n << " p: " << p << " s: " << s << std::endl;
        std::shared_ptr<Sealib::UndirectedGraph> g =
            Sealib::GraphCreator::randomUndirected(n, p, s);
        std::cout << "Done! Writing to file " << std::filesystem::absolute(path) << " " << std::endl;
        write(g, path);
    } else {
        auto tmp = get_opt(argv, argv + argc, "-f");
        if(tmp == nullptr) {
            print_help(argv[0]);
            return 1;
        }
        std::filesystem::path path(tmp);

        std::shared_ptr<Sealib::UndirectedGraph> graph = read(path);
        auto results = measureSubgraphStack<Sealib::SubGraphStack>(graph);
        auto simple_results = measureSubgraphStack<Sealib::SimpleSubGraphStack>(graph);
        std::cout << "SubGraphStack" << std::endl
                  << "Time: " << results.duration << " ms"
                  << " Memory: " << results.memory << std::endl;
        std::cout << "SimpleSubGraphStack" << std::endl
                  << "Time: " << simple_results.duration << " ms"
                  << " Memory: " << simple_results.memory << std::endl;
    }
}
