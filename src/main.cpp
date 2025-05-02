#include "graph_loader.h"
#include "interest_vector.h"
#include "partition.h"
#include "influence.h"
#include "seed_selection.h"  // << NEW
#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>

int main() {
    constexpr uint32_t MAX_TEST_NODES = 10000; // Limit for testing

    constexpr uint32_t MAX_ID = MAX_TEST_NODES - 1;
    constexpr size_t N = MAX_TEST_NODES;

    // constexpr uint32_t MAX_ID = 456626;
    // constexpr size_t   N      = MAX_ID + 1;

    std::cout << "[LOG] Initializing graph with " << N << " nodes\n";
    std::vector<Node> graph(N);

    // 1) Generate interest vectors
    std::cout << "[LOG] Generating interest vectors\n";
    generateInterestVectors(graph);

    // 2) Load graph layers
    std::cout << "[LOG] Loading graph layers\n";
    loadGraph(graph);
    std::cout << "[LOG] Graph load complete. Ready for SCC/CAC partitioning.\n";

    // 3) Compute SCC/CAC partitioning
    std::cout << "[LOG] Computing SCC/CAC partitioning...\n";
    computeSCC_CAC(graph);
    std::cout << "[LOG] Partitioning complete.\n";

    // (Optional) Report total components
    int maxCID = 0;
    for (auto &n : graph)
        if (n.compID > maxCID) maxCID = n.compID;
    std::cout << "[LOG] Found " << (maxCID + 1) << " components\n";

    // 4) Compute Influence Power (Algorithm 5)
    std::cout << "[LOG] Computing influence power...\n";
    std::array<double, NUM_LAYERS> alpha = {0.0, 0.50, 0.15, 0.35};  // α weights for RETWEET, MENTION, REPLY
    double damping = 0.85;
    std::vector<double> IP = computeInfluencePower(graph, alpha, damping);
    std::cout << "[LOG] Influence power computation complete.\n";

    // 5) Seed Candidate Selection (Algorithm 6)
    std::cout << "[LOG] Selecting seed candidates (Algorithm 6)...\n";
    std::vector<int> I_star = selectSeedCandidates(graph, IP);

    // 6) Final Seed Selection (Algorithm 7)
    std::cout << "[LOG] Final influential seed selection (Algorithm 7)...\n";
    std::vector<int> INF = selectFinalSeeds(graph, IP, I_star);

    // 7) Print top 10 influential seeds
    std::cout << "[LOG] Top 10 final seed nodes:\n";
    for (int i = 0; i < std::min(10, (int)INF.size()); ++i) {
        int u = INF[i];
        std::cout << "  Node " << u << " → IP = " << std::setprecision(6) << IP[u] << "\n";
    }

    std::cout << "[LOG] All stages complete.\n";
    return 0;
}
