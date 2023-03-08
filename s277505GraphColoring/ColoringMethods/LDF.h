#ifndef PERELLIGRAPHCOLORING_LDF_H
#define PERELLIGRAPHCOLORING_LDF_H

#include "Solver.h"

class LDF : public Solver {

    uint32_t num_threads;

    std::vector<uint32_t> wrong_ones;

    void coloring_in_parallel(uint32_t from, uint32_t to, Graph &graph);

public:
    LDF(int num_threads);

    std::string name() const;

    void solve(Graph &);
};

#endif //PERELLIGRAPHCOLORING_LDF_H
