#ifndef GRAPHCOLORINGPERELLI_GRAPH_H
#define GRAPHCOLORINGPERELLI_GRAPH_H

#include <bitset>
#include <cstdint>
#include <vector>

using adjacency_vector_t = std::vector<uint32_t>;
using adjacency_list_t = std::vector<uint32_t>;
using color_t = uint32_t;

class Graph {

    std::vector<uint32_t> neighbors;
    std::vector<adjacency_list_t> neighbor_indices;
    std::vector<color_t> colors;
    std::vector<uint32_t> deletedVertices;

    uint32_t deletedVerticesCount = 0;

    friend class DimacsP;
    friend class Luby;

public:
    Graph(const std::vector<adjacency_list_t> &adj_list);

    bool is_coloring_ok() const;
    uint32_t colors_num() const;

    uint32_t vertices_num() const;
    color_t color_of(uint32_t v) const;
    adjacency_list_t neighbors_of(uint32_t v) const;
    uint32_t degree_of(uint32_t v) const;

    void delete_vertex(uint32_t v);
    bool is_deleted(uint32_t v) const;
    bool empty() const;

    color_t color_with_smallest(uint32_t v);

    void clear();
};

#endif //GRAPHCOLORINGPERELLI_GRAPH_H
