#ifndef PERELLIGRAPHCOLORING_GREEDY_H
#define PERELLIGRAPHCOLORING_GREEDY_H

#include "Solver.h"
#include <random>

class Greedy : public Solver {
    std::mt19937 random_gen;

public:
    Greedy();

    std::string name() const;

    void solve(Graph &);
};

#endif //PERELLIGRAPHCOLORING_GREEDY_H
