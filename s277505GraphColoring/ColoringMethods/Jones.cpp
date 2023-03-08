#include "Jones.h"

Jones::Jones(int num_threads) : num_threads(num_threads), gen(RANDOM_SEED) {}

std::string Jones::name() const {
    return "Jones (" + std::to_string(num_threads) + " threads)";
}

void Jones::solve(Graph &graph) {

    std::vector<uint32_t> randomWeights(graph.vertices_num());
    std::iota(randomWeights.begin() + 1, randomWeights.end(), 1);
    std::shuffle(randomWeights.begin(), randomWeights.end(), gen);

    ProdConsV<uint32_t> free_vertices;
    std::atomic<uint32_t> num_vertices_uncolored = graph.vertices_num();

    std::vector<std::atomic<int>> waitlist(graph.vertices_num());
    for (uint32_t vertex = 0; vertex < graph.vertices_num(); ++vertex) {
        const auto &neighbors = graph.neighbors_of(vertex);
        auto current_weight = randomWeights[vertex];
        int num_wait = 0;
        for (const auto neighbor : graph.neighbors_of(vertex))
            if (randomWeights[neighbor] > current_weight)
                num_wait++;
        waitlist[vertex] = num_wait;
        if (num_wait == 0)
            free_vertices.push(vertex);
    }

    free_vertices.onReceive(num_threads, [&free_vertices, &num_vertices_uncolored, &graph, &waitlist, &randomWeights](uint32_t cb_vertex) {
        if (--num_vertices_uncolored == 0)
            free_vertices.stop();

        graph.color_with_smallest(cb_vertex);
        for (uint32_t neighbor : graph.neighbors_of(cb_vertex)) {
            if (randomWeights[cb_vertex] > randomWeights[neighbor] && --waitlist[neighbor] == 0)
                free_vertices.push(neighbor);
        }
    });

    free_vertices.join();
}