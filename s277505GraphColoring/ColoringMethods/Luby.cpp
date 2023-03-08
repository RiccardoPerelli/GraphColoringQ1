#include <iostream>
#include "Luby.h"

Luby::Luby(int num_threads) : num_threads(num_threads), partial_Selection(num_threads), kill_threads(false),
                                          V_splitter(nullptr), gen(RANDOM_SEED) {
    pthread_barrier_init(&thread_start_barrier, nullptr, num_threads + 1);
    pthread_barrier_init(&thread_end_barrier, nullptr, num_threads + 1);
}

Luby::~Luby() {
    pthread_barrier_destroy(&thread_start_barrier);
    pthread_barrier_destroy(&thread_end_barrier);
    delete V_splitter;
}

void Luby::solve(Graph &graph) {

    Graph uncolored_graph(graph);

    for (int i = 0; i < num_threads; i++)
        threads.emplace_back([&](int thread_idx, const Graph &graph) {
                                 while (true) {
                                     pthread_barrier_wait(&thread_start_barrier);
                                     if (kill_threads)
                                         return;
                                     auto partial_V = V_splitter->get(thread_idx);
                                     if (!partial_V.empty()) {
                                         auto &S = partial_Selection[thread_idx];
                                         while (S.empty()) {
                                             for (uint32_t vertex : partial_V) {
                                                 double probability = 1. / (2 * graph.degree_of(vertex));
                                                 std::bernoulli_distribution d(probability);
                                                 if (d(gen)) {
                                                     S.push_back(vertex);
                                                     Selection_bitmap[vertex] = true;
                                                 }
                                             }
                                         }
                                     }
                                     pthread_barrier_wait(&thread_end_barrier);
                                 }
                             },
                             i, std::cref(graph));

    color_t color = 0;
    while (!uncolored_graph.empty()) {
        compute_MIS(uncolored_graph);
        for (uint32_t vertex : MIS) { //con questo loop coloro i vertici del grafo, praticamente non uso mai color_with_smallest
            graph.colors[vertex] = color;
            uncolored_graph.delete_vertex(vertex);
        }
        color++;
    }

    kill_threads = true;
    pthread_barrier_wait(&thread_start_barrier);
    for (auto &t : threads)
        t.join();
}

void Luby::compute_MIS(const Graph &del_graph) {
    MIS.clear();
    V.clear();

    const Graph &graph = del_graph;

    for (uint32_t i = 0; i < graph.vertices_num(); i++)
        if (!del_graph.is_deleted(i))
            V.emplace(i);

    while (!V.empty()) {

        select_vertex(graph);

        remove_edges(graph);

        for (uint32_t v : Solver) {
            if (!Selection_bitmap[v])
                continue;
            MIS.emplace_back(v);
            V.erase(v);
            for (uint32_t neighbor : graph.neighbors_of(v))
                V.erase(neighbor);
        }
    }
}

void Luby::select_vertex(const Graph &graph) {
    Solver.clear();
    Selection_bitmap.clear();
    Selection_bitmap.resize(graph.vertices_num());
    for (auto &S_ : partial_Selection)
        S_.clear();

    V_vec = std::move(std::vector<uint32_t>(V.cbegin(), V.cend()));
    if (V_splitter != nullptr)
        delete V_splitter;
    V_splitter = new VSplitter(V_vec, num_threads);

    pthread_barrier_wait(&thread_start_barrier);
    pthread_barrier_wait(&thread_end_barrier);
    for (const auto &partial : partial_Selection)
        Solver.insert(Solver.end(), partial.cbegin(), partial.cend());
}

void Luby::remove_edges(const Graph &g) {
    for (uint32_t from : Solver) {
        auto neighbors = g.neighbors_of(from);
        for (auto pos = std::lower_bound(neighbors.begin(), neighbors.end(), from); pos != neighbors.end(); ++pos) {
            uint32_t to = *pos;
            if (!Selection_bitmap[to])
                continue;
            if (g.degree_of(from) <= g.degree_of(to))
                Selection_bitmap[from] = false;
            else
                Selection_bitmap[to] = false;
        }
    }
}

std::string Luby::name() const {
    return "Luby (" + std::to_string(num_threads) + " threads)";
}
