#ifndef PERELLIGRAPHCOLORING_LUBY_H
#define PERELLIGRAPHCOLORING_LUBY_H

#include "../Utility/Splitter.h"
#include "Solver.h"
#include <random>
#include <set>
#include <thread>

class Luby : public Solver {
    int num_threads;

    std::mt19937 gen;
    std::vector<std::thread> threads;
    std::vector<uint32_t> MIS;

    std::vector<uint32_t> Solver;
    std::vector<char> Selection_bitmap;
    std::vector<std::vector<uint32_t>> partial_Selection;
    std::set<uint32_t> V;

    std::vector<uint32_t> V_vec;
    VSplitter *V_splitter;

    bool kill_threads;
    pthread_barrier_t thread_start_barrier{}, thread_end_barrier{};

    inline void select_vertex(const Graph &);
    inline void remove_edges(const Graph &);
    void compute_MIS(const Graph &src);

public:
    Luby(int num_threads);
    ~Luby();

    std::string name() const;

    void solve(Graph &) override;
};

#endif //PERELLIGRAPHCOLORING_LUBY_H
