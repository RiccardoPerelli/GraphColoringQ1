# Introduction
This repo contains the source code of the solution for the project *Parallel Graph Coloring*, final project for the exam System and device programming.
In graph theory, graph coloring is a special case of graph labeling; in which particular labels, traditionally called “colors”, are assigned to elements of a graph subject to certain constraints. The convention of using colors originates from coloring the countries of a map. In its simplest form, it is a way of coloring the vertices of a graph such that no two adjacent vertices are of the same color. This version of the problem is called “vertex coloring”. 
Graph coloring enjoys many practical applications as well as theoretical challenges. Many graph applications are based on vertex coloring and many graph properties are based on graph coloring. Moreover, vertex coloring is the most famous version of coloring since other coloring problems can be transformed into a vertex coloring instance. 
Following you can find a little documentation about the classes that compose the project.

# Splitter
The **Splitter** class is used to create a range of integer values. This range will be used by the coloring methods, in order to distribute the vertices of the graph among the threads in play.

# ProdConsV
The **ProdConsV** class is used to create a producer-consumer structure. It is used in some solvers to facilitate thread synchronization.

# DimacsP
The **DimacsP** class is used to parse .graph files, that is, to read these files in such a way as to serialize their contents, and store them in a C++ object (**Graph Class**) used to test the various coloring methods.

# Graph
The Graph class represents the parsed graph, and contains the necessary methods and attributes for graph manipulation/coloring. Here's a brief overview:

1. *neighbors*, a vector representing the neighbors of a node.
2. *neighbor_indices*, a vector of neighbors for each node.
3. *colors*, a vector of colors that determines how many colors to use for that graph.
4. *deletedVertices*, a vector for keeping track of information about deleted vertices.
5. *deletedVerticesCount*, counts the value of deleted vertices, necessary to check if all vertices have been colored.
6. *bool is_coloring_ok()*, checks that the graph coloring is correct.
7. *uint32_t colors_num()*, returns the number of colors in the graph.
8. *uint32_t vertices_num() const*, returns the number of vertices.
9. *color_t color_of(uint32_t v) const*, returns the color of a specific vertex.
10. *adjacency_list_t neighbors_of(uint32_t v) const*, returns all the neighbors of a vertex.
11. *uint32_t degree_of(uint32_t v) const*, returns the degree of a vertex (number of neighbors).
12. *void delete_vertex(uint32_t v)*, deletes a vertex, a method useful in algorithms such as Luby, where there is a need to create subsets of the graph for parallel coloring.
13. *bool is_deleted(uint32_t v) const*, checks if a specific vertex is still present in the graph.
14. *bool empty() const*, checks if the graph in question is empty, useful for convenience graphs in algorithms where it is necessary to create mirror graphs.
15. *color_t color_with_smallest(uint32_t v)*, colors vertex V with the smallest available color.
16. *void clear()*, resets the graph, make it usable in different algorithms.

# Solver
**Solver.h**, It serves as a base class for all the coloring methods that are used.

# Benchmark
**Benchmark**, in this class, everything necessary for benchmarking the different resolution methods with different numbers of threads is stored.

# Greedy
The **Greedy** algorithm, trivially, applies the method described in Allwright 1995. Basically, I take all the vertices of my graph, make a permutation of them, and iteratively go through all the vertices in the permutation. For each vertex (sequentially), I look at all the colors of its neighbors and apply the smallest color that I can find among the available set of colors to the current vertex.

# Jones
The **Jones** algorithm, as presented in the Allwright 1995 paper, implements a coloring method based on the weight of the vertices. Essentially, an independent set is constructed by exploiting the local maxima. These nodes will be colored with the minimum possible color in parallel, and (also in parallel) new local maxima will be added using a waitlist that keeps track of which vertices are still waiting to be colored.

# LDF
The **LDF (Largest-degree-first)** algorithm is fundamentally the same as Jones, only before coloring the vertices, I assign each thread a subset of the graph. The coloring method used is as follows: For each subset, I sort the vertices by degree, unfortunately, having separate sets for each thread can generate problems and therefore incorrect colorings (because the color_with_smallest() method is not atomic). So a piece regarding the correction of incorrect colors has been added.

# SDL
The **SDL (Smallest-degree-last)** algorithm is similar to LDF but with an improved weighting system. In particular, the algorithm works by passing through two phases:

- Weighting phase: all the node degrees are saved in a vector, the highest degree is calculated, for each vertex if it has a degree smaller than the current degree then I assign the current weight to the weight vector at the position of the vertex, then for each neighbor, I reduce the degree by one. I do this until the current degree is equal to the max degree.
- Coloring phase: once the vertices are sorted based on their weights (calculated in the previous step), I assign a color to each vertex in the same way as the LDF algorithm. Finally, I correct the coloring of the incorrect vertices.

# Luby
The **Luby** algorithm creates a convenient uncolored graph. For each thread, the following instructions are executed:

- Each thread takes its part of the graph on which it will work (as we have seen before), in this part of the graph, a set of vertices is randomly selected (using a Bernoulli distribution), in this way, the vertices of the set are randomly selected to be inserted into the sub-subset.
- This is done until the entire subset is randomly inserted into another container (a sort of shuffle dependent on the Luby formula).
- After that, the MIS (Maximal Independent Set) is calculated, which is done as follows: For each vertex, I check if it has not been eliminated, if it has not, then I add that vertex to a vector V. Then I go through that vector V, and at each iteration, I select a vertex and remove a branch.algorithm creates a convenient uncolored graph. For each thread, the following instructions are executed:

# Benchamrks
## rgg_n_2_15_s0.graph
|Graph	            |Vertices|Edges	|ColoringAlg|	Success	|Colors	|Time elapsed|
|-------------------|--------|------|-----------|-----------|-------|------------|
|rgg_n_2_15_s0.graph|	32768|	320496	|Greedy	            |1	|15|	74.8919|
|rgg_n_2_15_s0.graph|	32768|	320496	|Jones (1 threads)	|1	|15|	108.931|
|rgg_n_2_15_s0.graph|	32768|	320496	|Jones (2 threads)	|1	|15|	72.3736|
|rgg_n_2_15_s0.graph|	32768|	320496	|Jones (4 threads)	|1	|15|	49.1809|
|rgg_n_2_15_s0.graph|	32768|	320496	|Jones (8 threads)	|1	|15|	41.3722|
|rgg_n_2_15_s0.graph|	32768|	320496	|SDL (1 threads)	|1	|15|	125.125|
|rgg_n_2_15_s0.graph|	32768|	320496	|SDL (2 threads)	|1	|15|	73.3429|
|rgg_n_2_15_s0.graph|	32768|	320496	|SDL (4 threads)	|1	|15|	39.2021|
|rgg_n_2_15_s0.graph|	32768|	320496	|LDF (1 threads)	|1	|14|	230.332|
|rgg_n_2_15_s0.graph|	32768|	320496	|LDF (2 threads)	|1	|14|	108.094|
|rgg_n_2_15_s0.graph|	32768|	320496	|LDF (4 threads)	|1	|14|	52.3957|
|rgg_n_2_15_s0.graph|	32768|	320496	|Luby (1 threads)	|1	|15|	380.154|
|rgg_n_2_15_s0.graph|	32768|	320496	|Luby (2 threads)	|1	|14|	323.403|
|rgg_n_2_15_s0.graph|	32768|	320496	|Luby (4 threads)	|1	|15|	277.695|
## rgg_n_2_17_s0.graph
|Graph	            |Vertices|Edges	|ColoringAlg|	Success	|Colors	|Time elapsed|
|-------------------|--------|------|-----------|-----------|-------|------------|
|rgg_n_2_17_s0.graph	|131072	|1457522	|Greedy	            |1	|18|	388.456|
|rgg_n_2_17_s0.graph	|131072	|1457522	|Jones (1 threads)	|1	|17|	480.528|
|rgg_n_2_17_s0.graph	|131072	|1457522	|Jones (2 threads)	|1	|17|	292.526|
|rgg_n_2_17_s0.graph	|131072	|1457522	|Jones (4 threads)	|1	|17|	190.535|
|rgg_n_2_17_s0.graph	|131072	|1457522	|Jones (8 threads)	|1	|17|	148.469|
|rgg_n_2_17_s0.graph	|131072	|1457522	|SDL (1 threads)	|1	|17|	485.979|
|rgg_n_2_17_s0.graph	|131072	|1457522	|SDL (2 threads)	|1	|17|	245.219|
|rgg_n_2_17_s0.graph	|131072	|1457522	|SDL (4 threads)	|1	|17|	126.419|
|rgg_n_2_17_s0.graph	|131072	|1457522	|LDF (1 threads)	|1	|16|	1006.5|
|rgg_n_2_17_s0.graph	|131072	|1457522	|LDF (2 threads)	|1	|16|	497.066|
|rgg_n_2_17_s0.graph	|131072	|1457522	|LDF (4 threads)	|1	|16|	272.166|
|rgg_n_2_17_s0.graph	|131072	|1457522	|Luby (1 threads)	|1	|16|	1754.97|
|rgg_n_2_17_s0.graph	|131072	|1457522	|Luby (2 threads)	|1	|17|	1450.05|
|rgg_n_2_17_s0.graph	|131072	|1457522	|Luby (4 threads)	|1	|17|	1287.62|
## rgg_n_2_18_s0.graph
|Graph	            |Vertices|Edges	|ColoringAlg|	Success	|Colors	|Time elapsed|
|-------------------|--------|------|-----------|-----------|-------|------------|
|rgg_n_2_18_s0.graph|	262144|	3094582|	Greedy           	|1	|19	|767.638|
|rgg_n_2_18_s0.graph|	262144|	3094582|	Jones (1 threads)	|1	|19	|1028.98|
|rgg_n_2_18_s0.graph|	262144|	3094582|	Jones (2 threads)	|1	|19	|632.089|
|rgg_n_2_18_s0.graph|	262144|	3094582|	Jones (4 threads)	|1	|19	|416.536|
|rgg_n_2_18_s0.graph|	262144|	3094582|	Jones (8 threads)	|1	|19	|319.795|
|rgg_n_2_18_s0.graph|	262144|	3094582|	SDL (1 threads)	    |1	|19	|1064.98|
|rgg_n_2_18_s0.graph|	262144|	3094582|	SDL (2 threads)	    |1	|19	|550.134|
|rgg_n_2_18_s0.graph|	262144|	3094582|	SDL (4 threads)	    |1	|19	|286.605|
|rgg_n_2_18_s0.graph|	262144|	3094582|	LDF (1 threads)	    |1	|17	|2584.26|
|rgg_n_2_18_s0.graph|	262144|	3094582|	LDF (2 threads)	    |1	|17	|1234.22|
|rgg_n_2_18_s0.graph|	262144|	3094582|	LDF (4 threads)	    |1	|17	|608.627|
|rgg_n_2_18_s0.graph|	262144|	3094582|	Luby (1 threads)	|1	|19	|4364.44|
|rgg_n_2_18_s0.graph|	262144|	3094582|	Luby (2 threads)	|1	|19	|3952.27|
|rgg_n_2_18_s0.graph|	262144|	3094582|	Luby (4 threads)	|1	|19	|3366.48|
## rgg_n_2_19_s0.graph
|Graph	            |Vertices|Edges	|ColoringAlg|	Success	|Colors	|Time elapsed|
|-------------------|--------|------|-----------|-----------|-------|------------|
|rgg_n_2_19_s0.graph|	524288|	6539548|	Greedy	            |1	|19	|1694.93|
|rgg_n_2_19_s0.graph|	524288|	6539548|	Jones (1 threads)	|1	|20	|2162.09|
|rgg_n_2_19_s0.graph|	524288|	6539548|	Jones (2 threads)	|1	|20	|1313.15|
|rgg_n_2_19_s0.graph|	524288|	6539548|	Jones (4 threads)	|1	|20	|842.074|
|rgg_n_2_19_s0.graph|	524288|	6539548|	Jones (8 threads)	|1	|20	|639.839|
|rgg_n_2_19_s0.graph|	524288|	6539548|	SDL (1 threads)	    |1	|20	|2225.21|
|rgg_n_2_19_s0.graph|	524288|	6539548|	SDL (2 threads)	    |1	|20	|1147.71|
|rgg_n_2_19_s0.graph|	524288|	6539548|	SDL (4 threads)	    |1 	|20	|611.203|
|rgg_n_2_19_s0.graph|	524288|	6539548|	LDF (1 threads)	    |1	|19	|4970.99|
|rgg_n_2_19_s0.graph|	524288|	6539548|	LDF (2 threads)	    |1	|19	|2400.57|
|rgg_n_2_19_s0.graph|	524288|	6539548|	LDF (4 threads)	    |1	|19	|1241.99|
|rgg_n_2_19_s0.graph|	524288|	6539548|	Luby (1 threads)	|1	|19	|9824.69|
|rgg_n_2_19_s0.graph|	524288|	6539548|	Luby (2 threads)	|1	|19	|7938.92|
|rgg_n_2_19_s0.graph|	524288|	6539548|	Luby (4 threads)	|1	|19	|6968.77|
