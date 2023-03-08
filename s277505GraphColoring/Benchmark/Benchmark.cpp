#include "Benchmark.h"
#include "../ColoringMethods/Luby.h"
#include "../ColoringMethods/Greedy.h"
#include "../ColoringMethods/Jones.h"
#include "../ColoringMethods/LDF.h"
#include "../ColoringMethods/SDL.h"
#include <chrono>
#include <iostream>

Benchmark::Benchmark(Graph &g) : solvers({new Greedy(),
                                                 new Jones(1),
                                                 new Jones(2),
                                                 new Jones(4),
                                                 new Jones(8),

                                                 new SDL(1),
                                                 new SDL(2),
                                                 new SDL(4),

                                                 new LDF(1),
                                                 new LDF(2),
                                                 new LDF(4),

                                                 new Luby(1),
                                                 new Luby(2),
                                                 new Luby(4),

                                         }),
                                 graph(g) {}

void Benchmark::run() {
    printf("Graph,Vertices,Edges,ColoringAlg,Success,Colors,Time elapsed\n");
    std::ofstream outputFile;
    outputFile.open("results_rgg_n_2_19_s0.csv");
    outputFile << "Graph,Vertices,Edges,ColoringAlg,Success,Colors,Time elapsed\n";
    for (Solver *s : solvers) {
        struct result res = run_single(s);
        printf("%s,%d,%d,%s,%d,%d,%.2f\n",
               settings.parse_md->filename.c_str(), settings.parse_md->n_vertices, settings.parse_md->n_edges,
               s->name().c_str(),
               res.success,
               res.num_colors,
               res.milliseconds);
        outputFile << settings.parse_md->filename.c_str() << "," << settings.parse_md->n_vertices << ","
        << settings.parse_md->n_edges << "," << s->name().c_str() << "," << res.success << "," << res.num_colors << ","
        << res.milliseconds << "\n";
        std::cout.flush();
        delete s;
    }
    outputFile.close();
}

struct result Benchmark::run_single(Solver *solver) {

    auto t1 = std::chrono::high_resolution_clock::now();
    solver->solve(graph);
    auto t2 = std::chrono::high_resolution_clock::now();

    bool success = graph.is_coloring_ok();
    uint32_t num_colors = graph.colors_num();
    double milliseconds = std::chrono::duration<double, std::milli>(t2 - t1).count();

    graph.clear();
    return {success, num_colors, milliseconds};
}