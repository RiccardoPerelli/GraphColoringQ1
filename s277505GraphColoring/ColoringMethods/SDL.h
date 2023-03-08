#ifndef PERELLIGRAPHCOLORING_SDL_H
#define PERELLIGRAPHCOLORING_SDL_H

#include "../graph/Graph.h"
#include "Solver.h"

#include <iostream>

#include <algorithm>

#include <random>

#include <numeric>
#include <thread>
#include <vector>

class SDL : public Solver {

    uint32_t num_threads;

    std::vector<uint32_t> wrong_ones;

    void apply_weighting_phase(const Graph &graph, std::vector<uint32_t> &degrees, std::vector<uint32_t> &weights, uint32_t from, uint32_t to);

    void apply_coloring_phase(const std::vector<uint32_t> &degrees, uint32_t vertex, uint32_t range, Graph &graph);

public:
    SDL(int num_threads);

    std::string name() const;

    void solve(Graph &);
};

#endif //PERELLIGRAPHCOLORING_SDL_H
