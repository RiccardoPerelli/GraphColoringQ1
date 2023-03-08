#include <string>
#include "SDL.h"
#include "../Utility/Splitter.h"

SDL::SDL(int num_threads) : num_threads(num_threads) {}

void SDL::solve(Graph &graph) {

    uint32_t num_vertices = graph.vertices_num();

    std::vector<uint32_t> degrees(num_vertices, 0);
    std::vector<uint32_t> weights(num_vertices, 0);

    Splitter rs(num_vertices, num_threads);

    std::vector<std::thread> threads;

    for (int thread_idx = 0; thread_idx < num_threads; thread_idx++) {
        threads.emplace_back(std::thread([&graph, &weights, &degrees, this, rs, thread_idx]() {
            uint32_t from = rs.get_min(thread_idx), to = rs.get_max(thread_idx);

            apply_weighting_phase(graph, degrees, weights, from, to);

            apply_coloring_phase(degrees, from, to, graph);
        }));
    }

    for (auto &th : threads) {
        th.join();
    }

    for (const auto &vertex : wrong_ones) {
        graph.color_with_smallest(vertex);
    }
}

void SDL::apply_weighting_phase(const Graph &graph, std::vector<uint32_t> &degrees, std::vector<uint32_t> &weights, uint32_t from, uint32_t to) {

    for (uint32_t vertexID = from; vertexID < to; vertexID++) {
        degrees[vertexID] = graph.degree_of(vertexID);
    }

    uint32_t max_degree = 0;
    for (uint32_t vertexID = from; vertexID < to; vertexID++)
        max_degree = std::max(max_degree, degrees[vertexID]);

    uint32_t CurrentWeight = 1;
    uint32_t CurrentDegree = 0;

    while (CurrentDegree <= max_degree) {
        for (uint32_t vertexID = from; vertexID < to; vertexID++) {
            if (degrees[vertexID] <= CurrentDegree && degrees[vertexID] >= 0) {
                weights[vertexID] = CurrentWeight;
                degrees[vertexID] = -1;
                for (uint32_t neighbor : graph.neighbors_of(vertexID)) {
                    if (degrees[neighbor] > 0) {
                        degrees[neighbor]--;
                    }
                }
            }
        }

        CurrentWeight++;
        CurrentDegree++;
    }
}

void SDL::apply_coloring_phase(const std::vector<uint32_t> &weights, uint32_t from, uint32_t to, Graph &graph) {

    std::vector<uint32_t> vertices_to_color(to - from);
    std::iota(vertices_to_color.begin(), vertices_to_color.end(), from);

    std::stable_sort(vertices_to_color.begin(), vertices_to_color.end(), [&](int i, int j) {
        return weights[i] > weights[j];
    });

    for (const auto &vertex_to_color : vertices_to_color) {
        uint32_t my_color = graph.color_with_smallest(vertex_to_color);
        for (const auto &neighbor : graph.neighbors_of(vertex_to_color)) {
            if (my_color == graph.color_of(neighbor)) {
                wrong_ones.emplace_back(vertex_to_color);
                break;
            }
        }
    }
}

std::string SDL::name() const {
    return "SDL (" + std::to_string(num_threads) + " threads)";
}