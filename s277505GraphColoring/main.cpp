#include "benchmark/Benchmark.h"
#include "Parser/DimacsP.h"
#include <algorithm>
#include <iostream>

int main(int argc, char **argv) {

    std::vector<std::string> args(argv, argv + argc);
    if (args.size() < 1) {
        std::cerr << "Syntax: " << args[0] << " <graph>" << std::endl;
        return 1;
    }

    std::ifstream file(args.back());

    DimacsP p(file, args.back());

    Graph graph = p.parse();

    Benchmark bench(graph);

    bench.settings.parse_md = &p.metadata;

    bench.run();
    return 0;
}