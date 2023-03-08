#ifndef PERELLIGRAPHCOLORING_JONES_H
#define PERELLIGRAPHCOLORING_JONES_H

#include "../Utility/ProdConsV.h"
#include "Solver.h"
#include <atomic>
#include <functional>
#include <map>
#include <random>
#include <thread>

class Jones : public Solver {
    int num_threads;
    std::mt19937 gen;

public:
    Jones(int num_threads = 1);

    std::string name() const;

    void solve(Graph &);
};

#endif //PERELLIGRAPHCOLORING_JONES_H
