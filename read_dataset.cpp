#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <sstream>

//─── configure layers and file paths ────────────────────────────────────────────
enum Layer : uint8_t { FOLLOW=0, RETWEET=1, MENTION=2, REPLY=3, NUM_LAYERS };
static const char* layer_names[NUM_LAYERS] = {
    "FOLLOW", "RETWEET", "MENTION", "REPLY"
};
static const std::array<std::string,NUM_LAYERS> files = {
    "Dataset-Higgs-Twitter/higgs-social_network.edgelist",
    "Dataset-Higgs-Twitter/higgs-retweet_network.edgelist",
    "Dataset-Higgs-Twitter/higgs-mention_network.edgelist",
    "Dataset-Higgs-Twitter/higgs-reply_network.edgelist"
};

//─── per-edge record ───────────────────────────────────────────────────────────
struct Edge {
    uint32_t to;
    float    weight;
    uint32_t timestamp;
};

//─── per-node adjacency lists + Tarjan/CAC metadata ─────────────────────────────
struct Node {
    std::array<std::vector<Edge>, NUM_LAYERS> out;
    int index   = -1;
    int lowlink = -1;
    int compID  = -1;
    int level   = 0;
    bool onStack = false;
    enum Type{UNCLASSIFIED,SCC,CAC} type = UNCLASSIFIED;
};

int main() {
    constexpr uint32_t MAX_ID = 456626;
    constexpr size_t   N      = MAX_ID + 1;

    std::cout << "[LOG] Initializing graph with " << N << " nodes\n";
    std::vector<Node> graph(N);

    //─── one-pass load for each layer ────────────────────────────────────────────
    for(int L = 0; L < NUM_LAYERS; ++L) {
        std::cout << "[LOG] Loading layer " << layer_names[L]
                  << " from \"" << files[L] << "\"\n";

        std::ifstream in(files[L]);
        if(!in) {
            std::cerr << "[ERROR] Cannot open " << files[L] << "\n";
            return 1;
        }

        uint32_t u, v;
        float    w;
        std::string line;
        uint64_t count = 0;

        while(std::getline(in, line)) {
            if(line.empty() || line[0]=='#') continue;
            std::istringstream ss(line);
            ss >> u >> v;
            if(!(ss >> w)) w = 1.0f;  // default

            if(u >= N || v >= N) {
                std::cerr << "[WARN] edge ("<<u<<","<<v<<") out of range, skipping\n";
                continue;
            }

            graph[u].out[L].push_back({v, w, 0});
            if(++count % 1000000 == 0) {
                std::cout << "[LOG]   " << count
                          << " edges loaded in " << layer_names[L] << "\n";
            }
        }
        std::cout << "[LOG] Completed layer " << layer_names[L]
                  << ": total edges = " << count << "\n";
    }

    //─── sample sanity check ──────────────────────────────────────────────────────
    uint32_t sample = 12345 < N ? 12345 : N-1;
    std::cout << "[LOG] Sample RETWEET edges from node " << sample << ":\n";
    int printed = 0;
    for(auto &e : graph[sample].out[RETWEET]) {
        std::cout << "    -> " << e.to << " (w=" << e.weight << ")\n";
        if(++printed >= 10) { std::cout << "    ...\n"; break; }
    }
    if(printed==0) std::cout << "    [none]\n";

    std::cout << "[LOG] Graph load complete. Ready for SCC/CAC partitioning.\n";
    return 0;
}
