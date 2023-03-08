#ifndef PERELLIGRAPHCOLORING_BENCHMARK_H
#define PERELLIGRAPHCOLORING_BENCHMARK_H

#include "../Parser/DimacsP.h"
#include "../ColoringMethods/Solver.h"

struct result {
    bool success;
    uint32_t num_colors;
    double milliseconds;
};

class Benchmark {

    //vettore dei metodi di risoluzione
    std::vector<Solver *> solvers;

    //Grafo su cui lavoro
    Graph graph;

    // metodo utilizzato per testare un singolo metodo di colorazione
    // restituisce una struct che conterrà le statistiche di risoluzione
    struct result run_single(Solver *);

public:
    Benchmark(Graph &);

    // Metodo che verrà richiamato nel main.cpp
    void run();

    struct {
        DimacsP::metadata_t *parse_md;
    } settings;
};

#endif //PERELLIGRAPHCOLORING_BENCHMARK_H
