#ifndef GRAPHCOLORINGPERELLI_DIMACSP_H
#define GRAPHCOLORINGPERELLI_DIMACSP_H

#include "../graph/Graph.h"
#include "../Utility/ProdConsV.h"
#include <fstream>
#include <thread>
#include <vector>

class DimacsP {
    std::ifstream file;

    using position_t = std::vector<adjacency_vector_t>::iterator;
    // Store the string to be parsed and the position where to put it
    using message_t = std::pair<std::string, position_t>;
    ProdConsV<message_t> queue;

    // Parse a line into a vector of numbers
    static std::vector<uint32_t> parse_numbers(const std::string &line, bool is_header = false);

public:
    using metadata_t = struct {
        std::string filename;
        int n_vertices, n_edges;
    };

    metadata_t metadata;

    DimacsP(std::ifstream &, const std::string &_path);

    Graph parse();
};

#endif //GRAPHCOLORINGPERELLI_DIMACSP_H
