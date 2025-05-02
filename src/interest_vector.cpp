#include "interest_vector.h"
#include <random>
#include <iostream>

void generateInterestVectors(std::vector<Node>& graph) {
    std::mt19937 gen(42);  // fixed seed for reproducibility
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    std::cout << "[LOG] Generating " << D << "-dimensional interest vectors\n";
    for (size_t u = 0; u < graph.size(); ++u) {
        auto& vec = graph[u].interest;
        vec.resize(D);
        float sum = 0.0f;
        for (int i = 0; i < D; ++i) {
            vec[i] = dist(gen);
            sum += vec[i];
        }
        for (int i = 0; i < D; ++i)
            vec[i] /= sum;
    }
    std::cout << "[LOG] Interest vectors assigned\n";
}