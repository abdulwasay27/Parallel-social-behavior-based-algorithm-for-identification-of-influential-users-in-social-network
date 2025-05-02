#include "graph_loader.h"
#include "interest_vector.h"
#include <iostream>
#include <vector>

int main() {
    constexpr uint32_t MAX_ID = 456626;
    constexpr size_t N = MAX_ID + 1;

    std::cout << "[LOG] Initializing graph with " << N << " nodes\n";
    std::vector<Node> graph(N);

    // Generate interest vectors
    generateInterestVectors(graph);

    // Load graph layers
    loadGraph(graph);

    std::cout << "[LOG] Graph load complete. Ready for SCC/CAC partitioning.\n";
    return 0;
}