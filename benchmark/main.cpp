#include <sealib/graph/graphcreator.h>
#include "trackingallocator.hpp"
#include "cmdoptions.hpp"
#include "io.hpp"
#include <iostream>
#include <filesystem>

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
    }
}

