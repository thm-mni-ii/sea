#include "./test_algorithms.h"
#include <sys/resource.h>
#include <cstdio>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include "../src/bfs/simplebfs.h"
#include "../src/collection/simplesequence.h"
#include "../src/dfs/simplereversedfs.h"
#include "../src/graph/simplevirtualgraph.h"
#include "../src/marker/simplecutvertexiterator.h"
#include "../src/planar/simpleouterplanarchecker.h"
#include "sealib/_types.h"
#include "sealib/collection/blockbitset.h"
#include "sealib/dictionary/choicedictionary.h"
#include "sealib/graph/graphcreator.h"
#include "sealib/graph/graphio.h"
#include "sealib/iterator/bfs.h"
#include "sealib/iterator/cutvertexiterator.h"
#include "sealib/iterator/dfs.h"
#include "sealib/iterator/outerplanarchecker.h"
#include "sealib/iterator/reversedfs.h"
#include "sealib/runtimetest.h"

namespace Sealib {

template <class F1, class F2, class G>
void AlgorithmComparison::measureTime(F1 f1, F2 f2, std::string file1,
                                      std::string file2, G fg, uint64_t from,
                                      uint64_t to) {
    RuntimeTest t1, t2;
    for (uint64_t run = 0; run < 5; run++) {
        std::vector<uint64_t> order;
        std::vector<std::future<double>> r1, r2;
        std::vector<std::thread> th1, th2;
        uint64_t step = from;
        for (uint64_t n = from; n <= to; n += step) {
            if (n >= 10 * step) step *= 10;
            order.push_back(n);
        }
        for (uint64_t n : order) {
            auto g = fg(n);
            std::promise<double> p1;
            r1.push_back(p1.get_future());
            th1.push_back(std::thread(measureFunction<F1, decltype(g)>, f1, g,
                                      std::move(p1)));
            std::promise<double> p2;
            r2.push_back(p2.get_future());
            th2.push_back(std::thread(measureFunction<F2, decltype(g)>, f2, g,
                                      std::move(p2)));
        }
        for (uint64_t a = 0; a < r1.size(); a++) {
            r1[a].wait();
            th1[a].join();
            t1.addLine(order[a], 0, r1[a].get());
            t1.saveCSV(file1);
            r2[a].wait();
            th2[a].join();
            t2.addLine(order[a], 0, r2[a].get());
            t2.saveCSV(file2);
        }
    }
    t1.printResults();
    t1.saveCSV(file1);
    printf("-----\n");
    t2.printResults();
    t2.saveCSV(file2);
}

template <class F, class G>
void AlgorithmComparison::measureFunction(F f, G g, std::promise<double> r) {
    struct rusage u1, u2;
    getrusage(RUSAGE_THREAD, &u1);
    f(g);
    getrusage(RUSAGE_THREAD, &u2);
    double t1 = u1.ru_utime.tv_sec + u1.ru_utime.tv_usec / 1e6,
           t2 = u2.ru_utime.tv_sec + u2.ru_utime.tv_usec / 1e6;
    r.set_value(t2 - t1);
}

template <class F1, class F2, class G>
void AlgorithmComparison::measureSpace(F1 f1, F2 f2, std::string file1, std::string file2,
                         G fg, uint64_t from, uint64_t to) {
    RuntimeTest t1, t2;
    for (uint64_t run = 0; run < 5; run++) {
        std::vector<uint64_t> order;
        std::vector<std::future<double>> r1, r2;
        std::vector<std::thread> th1, th2;
        uint64_t step = from;
        for (uint64_t n = from; n <= to; n += step) {
            if (n >= 10 * step) step *= 10;
            order.push_back(n);
        }
        for (uint64_t n : order) {
            auto g = fg(n);
            std::promise<double> p1;
            r1.push_back(p1.get_future());
            th1.push_back(std::thread(evalFunction<F1, decltype(g)>, f1, g,
                                      std::move(p1)));
            std::promise<double> p2;
            r2.push_back(p2.get_future());
            th2.push_back(std::thread(evalFunction<F2, decltype(g)>, f2, g,
                                      std::move(p2)));
        }
        for (uint64_t a = 0; a < r1.size(); a++) {
            r1[a].wait();
            th1[a].join();
            t1.addLine(order[a], 0, r1[a].get() / (1 << 10));
            t1.saveCSV(file1, "order,size,memory");
            r2[a].wait();
            th2[a].join();
            t2.addLine(order[a], 0, r2[a].get() / (1 << 10));
            t2.saveCSV(file2, "order,size,memory");
        }
    }
    t1.printResults();
    t1.saveCSV(file1, "order,size,memory");
    printf("-----\n");
    t2.printResults();
    t2.saveCSV(file2, "order,size,memory");
}

template <class F, class G>
void AlgorithmComparison::evalFunction(F f, G g, std::promise<double> r) {
    r.set_value(f(g));
}

#define Space_DDFS(G)                                                       \
    measureSpace(                                                           \
        [](Graph const& g) {                                                \
            uint64_t n = g.getOrder();                                      \
            std::vector<uint8_t> color(n);                                  \
            std::stack<std::pair<uint64_t, uint64_t>> s;                    \
            uint64_t smax = 0;                                              \
            for (uint64_t u = 0; u < n; u++) {                              \
                if (color[u] == DFS_WHITE) {                                \
                    DFS::visit_standard(                                    \
                        u, g, &color, &s,                                   \
                        [&s, &smax](uint64_t) {                             \
                            if (s.size() > smax) smax = s.size();           \
                        },                                                  \
                        DFS_NOP_EXPLORE, DFS_NOP_EXPLORE, DFS_NOP_PROCESS); \
                }                                                           \
            }                                                               \
            return color.capacity() * sizeof(uint8_t) +                     \
                   smax * sizeof(std::pair<uint64_t, uint64_t>);            \
        },                                                                  \
        [](Graph const& g) {                                                \
            uint64_t n = g.getOrder();                                      \
            CompactArray color(n, 3);                                       \
            ExtendedSegmentStack s(n, g, &color);                           \
            for (uint64_t a = 0; a < n; a++) {                              \
                if (color.get(a) == DFS_WHITE)                              \
                    DFS::visit_nloglogn(a, g, &color, &s, DFS::restore_top, \
                                        DFS_NOP_PROCESS, DFS_NOP_EXPLORE,   \
                                        DFS_NOP_EXPLORE, DFS_NOP_PROCESS);  \
            }                                                               \
            return color.byteSize() + s.byteSize();                         \
        },                                                                  \
        file1, file2, [](uint64_t n) { return (G); }, from, to)
#define Space_UDFS(G)                                      \
    measureSpace(                                          \
        [](UndirectedGraph const& g) {                     \
            SimpleSequence<uint8_t> color(g.getOrder());   \
            SimpleSequence<uint64_t> parent(g.getOrder()); \
            return color.byteSize() + parent.byteSize();   \
        },                                                 \
        [](UndirectedGraph const& g) {                     \
            uint64_t n = g.getOrder();                     \
            CompactArray color(n, 3);                      \
            StaticSpaceStorage parent(g);                  \
            return parent.byteSize() + color.byteSize();   \
        },                                                 \
        file1, file2, [](uint64_t n) { return (G); }, from, to)
#define Args_BFS(G)                                          \
    [](Graph const& g) {                                     \
        SimpleBFS b(g, BFS_NOP_PROCESS, BFS_NOP_EXPLORE);    \
        b.forEach([](std::pair<uint64_t, uint64_t>) {});     \
        return b.byteSize();                                 \
    },                                                       \
        [](Graph const& g) {                                 \
            BFS b(g, BFS_NOP_PROCESS, BFS_NOP_EXPLORE);      \
            b.forEach([](std::pair<uint64_t, uint64_t>) {}); \
            return b.byteSize();                             \
        },                                                   \
        file1, file2, [](uint64_t n) { return (G); }, from, to
#define Args_CV(G)                      \
    [](UndirectedGraph const& g) {      \
        SimpleCutVertexIterator b(g);   \
        b.forEach([](uint64_t) {});     \
        return b.byteSize();            \
    },                                  \
        [](UndirectedGraph const& g) {  \
            CutVertexIterator b(g);     \
            b.forEach([](uint64_t) {}); \
            return b.byteSize();        \
        },                              \
        file1, file2, [](uint64_t n) { return (G); }, from, to
#define Args_RDFS(G)                    \
    [](DirectedGraph const& g) {        \
        SimpleReverseDFS b(g);          \
        b.forEach([](UserCall) {});     \
        return b.byteSize();            \
    },                                  \
        [](DirectedGraph const& g) {    \
            ReverseDFS b(g);            \
            b.forEach([](UserCall) {}); \
            return b.byteSize();        \
        },                              \
        file1, file2, [](uint64_t n) { return (G); }, from, to
#define Args_OPG(G)                    \
    [](UndirectedGraph const& g) {     \
        SimpleOuterplanarChecker b(g); \
        b.isOuterplanar();             \
        return b.byteSize();           \
    },                                 \
        [](UndirectedGraph const& g) { \
            OuterplanarChecker b(g);   \
            b.isOuterplanar();         \
            return b.byteSize();       \
        },                             \
        file1, file2, [](uint64_t n) { return (G); }, from, to
#define Func_RandIndices(G)                                                \
    [](uint64_t n) {                                                       \
        auto g = (G);                                                      \
        std::random_device rnd;                                            \
        std::uniform_int_distribution<uint64_t> dist(0, g.getOrder() - 1); \
        std::vector<uint64_t> i;                                           \
        for (uint64_t a = 0; a < n; a++) {                                 \
            i.emplace_back(dist(rnd));                                     \
        }                                                                  \
        return std::pair<decltype(G), std::vector<uint64_t>>{g, i};        \
    }
#define Funcs_VirtualGraphSplit(G, S)                                         \
    measureSpace(                                                             \
        [](uint64_t n) { return std::vector<bool>(n).capacity() / 8; },       \
        [](uint64_t n) { return ChoiceDictionary(n).byteSize(); },            \
        "mem-vgra-std-vert-" S ".csv", "mem-vgra-eff-vert-" S ".csv",         \
        [](uint64_t n) { return n; }, from, to);                              \
    measureSpace(                                                             \
        [](UndirectedGraph const& g) {                                        \
            return g.getOrder() / 16 * sizeof(uint64_t);                      \
        },                                                                    \
        [](UndirectedGraph const& g) {                                        \
            return ChoiceDictionary(g.getOrder()).byteSize() +                \
                   g.getOrder() / 16 * sizeof(std::pair<uint64_t, uint64_t>); \
        },                                                                    \
        "mem-vgra-std-vAdj-" S ".csv", "mem-vgra-eff-vAdj-" S ".csv",         \
        [](uint64_t n) { return (G); }, from, to);                            \
    measureSpace([](UndirectedGraph const&) { return 0; },                    \
                 [](UndirectedGraph const& g) {                               \
                     uint64_t csize = 0;                                      \
                     for (uint64_t a = 0; a < g.getOrder(); a++) {            \
                         csize += ChoiceDictionary(g.deg(a)).byteSize();      \
                     }                                                        \
                     return csize;                                            \
                 },                                                           \
                 "/dev/null", "mem-vgra-eff-pAdj-" S ".csv",                  \
                 [](uint64_t n) { return (G); }, from, to);

int AlgorithmComparison::launch(std::string program, std::string file1,
                                std::string file2, uint64_t from, uint64_t to) {
    int r = 0;
    switch (program[0]) {
        case 'd':
            // runtime DDFS
            measureTime(
                [](Graph const& g) { DFS::standardDFS(g); },
                [](Graph const& g) { DFS::nloglognBitDFS(g); }, file1, file2,
                [](uint64_t n) { return GraphCreator::kOutdegree(n, 20); },
                from, to);
            break;
        case 'D':
            // space DDFS
            Space_DDFS(GraphCreator::kOutdegree(n, 20));
            break;
        case 'b':
            // runtime BFS
            measureTime(Args_BFS(GraphCreator::kOutdegree(n, 20)));
            break;
        case 'B':
            // space BFS
            measureSpace(Args_BFS(GraphCreator::kOutdegree(n, 20)));
            break;
        case 'c':
            // runtime CV
            measureTime(Args_CV(GraphCreator::sparseUndirected(n)));
            break;
        case 'C':
            // space CV
            measureSpace(Args_CV(GraphCreator::sparseUndirected(n)));
            break;
        case 'u':
            // runtime UDFS
            measureTime(
                [](UndirectedGraph const& g) { DFS::standardDFS(g); },
                [](UndirectedGraph const& g) { DFS::nplusmBitDFS(g); }, file1,
                file2,
                [](uint64_t n) { return GraphCreator::sparseUndirected(n); },
                from, to);
            break;
        case 'U':
            // space UDFS
            Space_UDFS(GraphCreator::sparseUndirected(n));
            break;
        case 'r':
            // runtime RDFS
            measureTime(Args_RDFS(GraphCreator::kOutdegree(n, 20)));
            break;
        case 'R':
            // space RDFS
            measureSpace(Args_RDFS(GraphCreator::kOutdegree(n, 20)));
            break;
        case 'o':
            // runtime OPG
            measureTime(Args_OPG(GraphCreator::triangulated(n)));
            break;
        case 'O':
            // space OPG
            measureSpace(Args_OPG(GraphCreator::triangulated(n)));
            break;
        case '/':
            // m/n variation tests
            switch (program[1]) {
                case 'd':
                    measureTime([](Graph const& g) { DFS::standardDFS(g); },
                                [](Graph const& g) { DFS::nloglognBitDFS(g); },
                                file1, file2,
                                [](uint64_t n) {
                                    return GraphCreator::kOutdegree(1e6, n);
                                },
                                from, to);
                    break;
                case 'D':
                    Space_DDFS(GraphCreator::kOutdegree(1e6, n));
                    break;
                case 'u':
                    measureTime(
                        [](UndirectedGraph const& g) { DFS::standardDFS(g); },
                        [](UndirectedGraph const& g) { DFS::nplusmBitDFS(g); },
                        file1, file2,
                        [](uint64_t n) {
                            return GraphCreator::kRegular(1e6, n);
                        },
                        from, to);
                    break;
                case 'U':
                    Space_UDFS(GraphCreator::kRegular(1e6, n));
                    break;
                case 'S':
                    // SSA mem split test
                    measureSpace(
                        [](UndirectedGraph const& g) {
                            return SimpleSequence<uint64_t>(g.getOrder())
                                .byteSize();
                        },
                        [](UndirectedGraph const& g) {
                            return StaticSpaceStorage(g).byteSize();
                        },
                        file1, file2,
                        [](uint64_t n) {
                            return GraphCreator::kRegular(1e6, n);
                        },
                        from, to);
                    break;
                case 'b':
                    measureTime(Args_BFS(GraphCreator::kOutdegree(1e6, n)));
                    break;
                case 'B':
                    measureSpace(Args_BFS(GraphCreator::kOutdegree(1e6, n)));
                    break;
                case 'r':
                    measureTime(Args_RDFS(GraphCreator::kOutdegree(1e6, n)));
                    break;
                case 'R':
                    measureSpace(Args_RDFS(GraphCreator::kOutdegree(1e6, n)));
                    break;
                case 'c':
                    measureTime(Args_CV(GraphCreator::kRegular(1e6, n)));
                    break;
                case 'C':
                    measureSpace(Args_CV(GraphCreator::kRegular(1e6, n)));
                    break;
                default:
                    r = 1;
                    break;
            }
            break;
        case '%':
            switch (program[1]) {
                case 'o':
                    measureTime(Args_OPG(GraphCreator::cycle(1e6, n)));
                    break;
                case 'O':
                    measureSpace(Args_OPG(GraphCreator::cycle(1e6, n)));
                    break;
                case 'c':
                    measureTime(Args_CV(GraphCreator::cycle(1e6, n)));
                    break;
                case 'C':
                    measureSpace(Args_CV(GraphCreator::cycle(1e6, n)));
                    break;
                default:
                    r = 1;
                    break;
            }
            break;
        case '+':
            switch (program[1]) {
                case 's':
                    measureTime(
                        [](std::pair<UndirectedGraph,
                                     std::vector<uint64_t>> const& p) {
                            UndirectedGraph const& g = p.first;
                            std::vector<uint64_t> const& i = p.second;
                            SimpleSequence<uint64_t> s(g.getOrder());
                            uint64_t a = 0, b = 0;
                            while (a < i.size()) {
                                if (a % 4 == 0) {
                                    s.insert(i[a], g.deg(i[a]) - 1);
                                } else {
                                    b += s.get(i[a]);
                                }
                                a++;
                            }
                        },
                        [](std::pair<UndirectedGraph,
                                     std::vector<uint64_t>> const& p) {
                            UndirectedGraph const& g = p.first;
                            std::vector<uint64_t> const& i = p.second;
                            StaticSpaceStorage s(g);
                            uint64_t a = 0, b = 0;
                            while (a < i.size()) {
                                if (a % 4 == 0) {
                                    s.insert(i[a], g.deg(i[a]) - 1);
                                } else {
                                    b += s.get(i[a]);
                                }
                                a++;
                            }
                        },
                        file1, file2,
                        Func_RandIndices(GraphCreator::sparseUndirected(1e6)),
                        from, to);
                    break;
                case 'S':
                    measureSpace(
                        [](UndirectedGraph const& g) {
                            SimpleSequence<uint64_t> s(g.getOrder());
                            return s.byteSize();
                        },
                        [](UndirectedGraph const& g) {
                            StaticSpaceStorage s(g);
                            return s.byteSize();
                        },
                        file1, file2,
                        [](uint64_t n) {
                            return GraphCreator::kRegular(n, 20);
                        },
                        from, to);
                    break;
                case 'g':
                    measureTime(
                        [](std::pair<UndirectedGraph,
                                     std::vector<uint64_t>> const& p) {
                            SimpleVirtualGraph g(p.first);
                            std::vector<uint64_t> const& i = p.second;
                            uint64_t a = 0, b = 0;
                            while (a < fmin(i.size() / 32, g.getOrder() / 32)) {
                                g.addEdge(i[a], i[a + 1]);
                                a += 2;
                            }
                            while (a < i.size()) {
                                if (a % 4 == 0) {
                                    b = g.head(i[a], g.deg(i[a]) - 1);
                                } else {
                                    b += g.deg(i[a]);
                                }
                                a++;
                            }
                        },
                        [](std::pair<UndirectedGraph,
                                     std::vector<uint64_t>> const& p) {
                            VirtualGraph g(p.first);
                            std::vector<uint64_t> const& i = p.second;
                            uint64_t a = 0, b = 0;
                            while (a < fmin(i.size() / 32, g.getOrder() / 32)) {
                                g.addEdge(i[a], i[a + 1]);
                                a += 2;
                            }
                            while (a < i.size()) {
                                if (a % 4 == 0) {
                                    b = g.head(i[a], g.deg(i[a]) - 1);
                                } else {
                                    b += g.deg(i[a]);
                                }
                                a++;
                            }
                        },
                        file1, file2,
                        Func_RandIndices(GraphCreator::sparseUndirected(1e6)),
                        from, to);
                    break;
                case 'G':
                    measureSpace(
                        [](UndirectedGraph const& g) {
                            return SimpleVirtualGraph(g).byteSize();
                        },
                        [](UndirectedGraph const& g) {
                            return VirtualGraph(g).byteSize();
                        },
                        file1, file2,
                        [](uint64_t n) {
                            return GraphCreator::sparseUndirected(n);
                        },
                        from, to);
                    break;
                case 'c':
                    measureTime(
                        [](std::pair<uint64_t, std::vector<uint64_t>> const&
                               p) {
                            std::vector<uint64_t> const& i = p.second;
                            SimpleSequence<uint8_t> s(p.first);
                            uint64_t b = 0;
                            for (uint64_t a : i) {
                                if (s.get(a) < DFS_BLACK) {
                                    s.insert(a, s.get(a) + 1);
                                } else {
                                    b += s.get(a);
                                }
                            }
                        },
                        [](std::pair<uint64_t, std::vector<uint64_t>> const&
                               p) {
                            std::vector<uint64_t> const& i = p.second;
                            CompactArray s(p.first);
                            uint64_t b = 0;
                            for (uint64_t a : i) {
                                if (s.get(a) < DFS_BLACK) {
                                    s.insert(a, s.get(a) + 1);
                                } else {
                                    b += s.get(a);
                                }
                            }
                        },
                        file1, file2,
                        [](uint64_t n) {
                            uint64_t length = 1e6;
                            std::random_device rnd;
                            std::uniform_int_distribution<uint64_t> dist(
                                0, length - 1);
                            std::vector<uint64_t> i;
                            for (uint64_t a = 0; a < n; a++) {
                                i.emplace_back(dist(rnd));
                            }
                            return std::pair<uint64_t, std::vector<uint64_t>>{
                                length, i};
                        },
                        from, to);
                    break;
                case 'i':
                    // on each found bit, insert 3 new bits until all found
                    measureTime(
                        [](std::vector<uint64_t> const& i) {
                            std::vector<bool> c(i.size());
                            uint64_t a = 0;
                            c[i[a]] = 1;
                            while (!c.empty()) {
                                for (uint64_t b = 0; b < c.size(); b++) {
                                    if (c[b]) {
                                        c[b] = 0;
                                        if (a < i.size()) {
                                            c[i[a + 1]] = 1;
                                            c[i[a + 2]] = 1;
                                            c[i[a + 3]] = 1;
                                            a += 3;
                                        }
                                        break;
                                    }
                                }
                            }
                        },
                        [](std::vector<uint64_t> const& i) {
                            ChoiceDictionary c(i.size());
                            ChoiceDictionaryIterator ci(c);
                            uint64_t a = 0, n = 1;
                            c.insert(i[a]);
                            while (n > 0) {
                                ci.init();
                                while (ci.more()) {
                                    c.remove(ci.next());
                                    if (a < i.size()) {
                                        c.insert(i[a + 1]);
                                        c.insert(i[a + 2]);
                                        c.insert(i[a + 3]);
                                        a += 3;
                                    }
                                    break;
                                }
                            }
                        },
                        file1, file2,
                        [](uint64_t n) {
                            std::random_device rnd;
                            std::uniform_int_distribution<uint64_t> dist(0,
                                                                         n - 1);
                            std::vector<uint64_t> i;
                            for (uint64_t a = 0; a < n; a++) {
                                i.emplace_back(dist(rnd));
                            }
                            return i;
                        },
                        from, to);
                    break;
                default:
                    r = 1;
                    break;
            }
            break;
        case ':':
            switch (program[1]) {
                case 'G':
                    // split mem test for virtual graphs
                    // emulates n/32 virtual edges
                    Funcs_VirtualGraphSplit(GraphCreator::triangulated(n),
                                            "triang");
                    break;
                case 'E':
                    // split mem test for edge marker
                    measureSpace(
                        [](UndirectedGraph const& g) {
                            std::vector<bool> bits;
                            for (uint64_t u = 0; u < g.getOrder(); u++) {
                                bits.push_back(1);
                                for (uint64_t k = 0; k < g.deg(u); k++) {
                                    bits.push_back(0);
                                }
                            }
                            RankSelect offset(Bitset<uint8_t>(std::move(bits)));
                            return offset.byteSize();
                        },
                        [](UndirectedGraph const& g) {
                            uint64_t m = 0;
                            for (uint64_t u = 0; u < g.getOrder(); u++) {
                                for (uint64_t k = 0; k < g.deg(u); k++) {
                                    m++;
                                }
                            }
                            return CompactArray(m).byteSize();
                        },
                        "mem-edge-offset.csv", "mem-edge-data.csv",
                        [](uint64_t n) {
                            return GraphCreator::kRegular(n, 20);
                        },
                        from, to);
                    measureSpace(
                        [](UndirectedGraph const& g) {
                            return StaticSpaceStorage(g).byteSize();
                        },
                        [](UndirectedGraph const& g) {
                            return std::vector<bool>(g.getOrder()).capacity() /
                                   8;
                        },
                        "mem-edge-parent.csv", "mem-edge-cc.csv",
                        [](uint64_t n) {
                            return GraphCreator::kRegular(n, 20);
                        },
                        from, to);
                    break;
                default:
                    r = 1;
                    break;
            }
            break;

        default:
            r = 1;
            break;
    }
    return r;
}

}  // namespace Sealib
