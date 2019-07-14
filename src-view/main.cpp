#include <sealib/_types.h>
#include <sealib/graph/graphcreator.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

int main() {
    Sealib::UndirectedGraph g = Sealib::GraphCreator::kRegular(20, 4);
    std::vector<bool> visited(g.getOrder());
    std::stack<std::pair<uint64_t, uint64_t>> stack;

    stack.emplace(0, 0);
    std::cout << "Vertex: " << 0 << std::endl;

    while (!stack.empty()) {
        auto entry = stack.top();
        stack.pop();
        stack.emplace(entry.first, entry.second + 1);
        visited[entry.first] = true;

        if (entry.second < g.deg(entry.first)) {
            stack.pop();
            continue;
        }
        if (visited[g.head(entry.first, entry.second)]) {
            continue;
        }
        stack.emplace(g.head(entry.first, entry.second), 0);
        std::cout << "Vertex: " << g.head(entry.first, entry.second)
                  << std::endl;
    }

    return 0;
}
