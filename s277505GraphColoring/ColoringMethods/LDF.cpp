#include "LDF.h"
#include "../Utility/Splitter.h"
#include <numeric>
#include <thread>
#include <stdexcept>
#include <iostream>

LDF::LDF(int num_threads) : num_threads(num_threads) {}

void LDF::solve(Graph &graph) {

    //Ogni thread lavora su un range specifico.
    Splitter rs(graph.vertices_num(), num_threads);

    std::vector<std::thread> threads;

    for (int thread_idx = 0; thread_idx < num_threads; thread_idx++)
        threads.emplace_back(std::thread([thread_idx, &graph, this, rs]() {

            // inizio e fine del range di operazione del thread.
            uint32_t from = rs.get_min(thread_idx), to = rs.get_max(thread_idx);

            coloring_in_parallel(from, to, graph);
        }));

    for (auto &th : threads) {
        th.join();
    }

    for (const auto &vertex : wrong_ones) {
        graph.color_with_smallest(vertex);
    }
}

void LDF::coloring_in_parallel(uint32_t from, uint32_t to, Graph &graph) {

    std::vector<uint32_t> vertices_to_color(to - from);
    std::iota(vertices_to_color.begin(), vertices_to_color.end(), from);

    std::stable_sort(vertices_to_color.begin(), vertices_to_color.end(), [&](int i, int j) {
        return graph.degree_of(i) > graph.degree_of(j);
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

std::string LDF::name() const {
    return "LDF (" + std::to_string(num_threads) + " threads)";
}