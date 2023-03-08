#include "Greedy.h"
#include <numeric>
#include <iostream>

Greedy::Greedy() : random_gen(RANDOM_SEED) {}

void Greedy::solve(Graph &graph) {

    // creo vettore di interi partendo dal numero di vertici del grafo
    std::vector<uint32_t> permutation(graph.vertices_num());

    // funzione che riempe il vettore con una serie di valori sequenziali e incrementali
    std::iota(permutation.begin(), permutation.end(), 0);

    // permuta i valori
    std::shuffle(permutation.begin(), permutation.end(), random_gen);

    // coloro con il colore minore per quel vertice.
    for (uint32_t index : permutation)
        graph.color_with_smallest(index);
}

std::string Greedy::name() const {
    return "Greedy";
}