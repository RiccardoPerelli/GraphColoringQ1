#include "Graph.h"
#include <cstddef>
#include <set>
#include <unordered_set>

Graph::Graph(const std::vector<std::vector<uint32_t>> &adj_list) : colors(adj_list.size()),
                                                                   neighbor_indices(adj_list.size()),
                                                                   deletedVertices(adj_list.size()){
    for (size_t idx = 0; idx < adj_list.size(); idx++){
        const auto &list = adj_list[idx];
        neighbors.insert(neighbors.end(), list.begin(), list.end());
        deletedVertices[idx] = 0;
    }

    auto begin_it = neighbors.begin();
    for (size_t idx = 0; idx < adj_list.size(); idx++){
        const auto &list = adj_list[idx];
        auto end_it = begin_it + list.size();
        neighbor_indices[idx] = std::vector<uint32_t>(begin_it, end_it);
        begin_it = end_it;
    }
}

bool Graph::is_coloring_ok() const {
    for(size_t idx = 0; idx < neighbor_indices.size(); idx++) {
        color_t from_color = color_of(idx);
        for(const uint32_t &to_idx : neighbors_of(idx)) {
            color_t to_color = color_of(to_idx);
            if(from_color == to_color)
                return false;
        }
    }
    return true;
}

uint32_t Graph::colors_num() const {
    return std::unordered_set<color_t>(this->colors.cbegin(), this->colors.cend()).size();
}

uint32_t Graph::vertices_num() const {
    return neighbor_indices.size();
}

color_t Graph::color_of(uint32_t v) const {
    return colors[v];
}

adjacency_list_t Graph::neighbors_of(uint32_t v) const {
    return neighbor_indices[v];
}

uint32_t Graph::degree_of(uint32_t v) const {
    return neighbors_of(v).size();
}

void Graph::clear() {
    std::fill(colors.begin(), colors.end(), 0);
    std::fill(deletedVertices.begin(), deletedVertices.end(), 0);
    deletedVerticesCount = 0;
}

color_t Graph::color_with_smallest(uint32_t v) {
    std::set<color_t> neighbor_colors;
    for (const auto &neighbor : neighbors_of(v))
        neighbor_colors.emplace(color_of(neighbor));

    color_t smallest_color = 0;
    for (uint32_t neighbor_color : neighbor_colors)
        if (smallest_color != neighbor_color)
            break;
        else
            smallest_color++;
    colors[v] = smallest_color;

    return smallest_color;
}

void Graph::delete_vertex(uint32_t v) {
    if(deletedVertices[v] == 0) {
        deletedVertices[v] = 1;
        deletedVerticesCount++;
    }
}

bool Graph::is_deleted(uint32_t v) const {
    return deletedVertices[v];
}

bool Graph::empty() const {
    return deletedVerticesCount == this->vertices_num();
}

