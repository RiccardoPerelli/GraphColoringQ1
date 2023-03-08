#ifndef PERELLIGRAPHCOLORING_SOLVER_H
#define PERELLIGRAPHCOLORING_SOLVER_H

#include "../graph/Graph.h"
#include <string>

#if WITH_RANDOM_SEED
#define RANDOM_SEED (std::random_device()())
#else
#define RANDOM_SEED 234
#endif

class Solver {
public:
    virtual ~Solver(){};

    virtual std::string name() const = 0;

    virtual void solve(Graph &) = 0;
};

#endif //PERELLIGRAPHCOLORING_SOLVER_H
