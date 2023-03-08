# Splitter
La classe **splitter** serve per creare un range di valori interi. il range sarà utilizzato dai metodi di colorazione,
in modo tale da spartire i vertici del grafo tra i thread in gioco.

# ProdConsV
La classe **ProdConsV** viene utilizzata per creare una struttura che serva da produttore consumatore. Utilizzata in alcuni risolutori per facilitare la sincronizzazione dei thread.

# DimacsP
La classe **DimacsP** viene utilizzata per fare il parsing dei file *.graph*, ovvero, leggere questi file in maniera tale da serializzarne
il contenuto, ed immagazzinarlo in un oggetto C++ (**Graph Class**)
utilizzato per testare i vari metodi di colorazione.

# Graph
La classe **Graph** rappresenta il grafo parsato, all'interno di essa sono presenti i metodi e gli 
attributi necessari alla manipolazione/colorazione del grafo. Ne facciamo una breve overview:
1.  *neighbors*, vettore che rappresenta i vicini di un nodo
2.  *neighbor_indices*, vettore di vicini per ogni nodo.
3.  *colors*, vettore di colori determina quanti colori utilizzare per quel grafo.
4.  *deletedVertices*, vettore per mantenere un'informazione circa i vertici eliminati.
5.  *deletedVerticesCount*, conta il valore dei vertici eliminati, necessario per fare il check se tutti sono stati colorati.
6.  *bool is_coloring_ok()*, verifica che la colorazione del grafo sia giusta.
7.  *uint32_t colors_num()*, ritorna il numero di colori del grafo.
8.  *uint32_t vertices_num() const*, ritorna il numero di vertici.
9. *color_t color_of(uint32_t v) const*, ritorna il colore di un vertice specifico.
10. *adjacency_list_t neighbors_of(uint32_t v) const*, ritorna tutti i vicini di un vertice.
11. *uint32_t degree_of(uint32_t v) const*, ritorna il grado di un vertice (numero di vicini).
12. *void delete_vertex(uint32_t v)*, elimina un vertice, metodo utile in algoritmi come Luby, dove è presente la necessità di 
	creare dei subset del grafo per la colorazione in parallelo.
13. *bool is_deleted(uint32_t v) const*, verifica che uno specifico vertice sia ancora o meno presente nel grafo.
14. *bool empty() const*, verifica che il grafo in questione sia vuoto, utile per grafi di comodo in algoritmi in cui è necessario creare dei grafi specchio.
15. *color_t color_with_smallest(uint32_t v)*, colora il vertice V con il minor colore presente.
17. *void clear()*, resetta il grafo, in modo che possa essere usato da un altro Algoritmo.

# Solver
**Solver.h**, fa da classe base per tutti i metodi di colorazione che sono utilizzati.

# Benchmark
**Benchmark**, in questa classe viene immagazzinato tutto il necessario per effettuare i benchmark sui diversi metodi di risoluzione e con diverso numero di thread.

# Greedy
L'algoritmo **Greedy**, banalmente, applica il metodo descritto in allwrigth 1995, praticamente prendo tutti i vertici del mio grafo, ne faccio una permutazione e iterativamente
scorro tutti i vertici facenti parte della permutazione, e per ognuno di essi (in modo sequenziale) cerco tutti i colori dei suoi vicini e applico al vertice corrente il colore
minore che posso trovare tra il set di colori disponibili.

# Jones
Algoritmo **Jones**, come presentato nel paper allwright1995 implementa un metodo di colorazione basato sul peso dei vertici
praticamente, viene costruito un indipendent set sfruttando i massimi locali, questi nodi verranno colorati con il minimo
colore possibile parallelamente e (sempre parallelamente) verranno aggiunti nuovi massimi locali sfruttando una waitList
che permette di mantenere un'informazione circa quali vertici sono ancora in attesa di essere colorati.

# LDF
Algoritmo **LDF (Largest-degree-first)**, Fondamentalmente uguale al Jones, solo che prima di colorare i vertici assegno ad ogni thread un sotto-set del grafo, e il metodo 
di colorazione utilizzato è il seguente:
Per ogni sotto-set faccio un sort che mi permettà di ordinare i vertici per grado, purtroppo, avendo dei set separati per ogni thread questo può generare dei problemi 
e quindi delle colorazioni errate (perchè il metodo color_with_smallest() non è atomico), quindi è stato aggiunto un pezzo relativo alla correzione dei colori errati.

# SDL
Algoritmo **SDL (Smallest-degree-last)**, simile al LDF ma con un sistema di weighting migliorato, in particolare l'algoritmo funziona passando attraverso due fasi:
-weighting phase: vengono salvati tutti i gradi dei nodi in un vettore, viene calcolato il grado più alto, per ogni vertice se questo ha un grado minore del grado corrente
allora assegno al vettore dei pesi in posizione del vertice il peso corrente, poi per ogni vicino riduco il grado di uno.
Faccio questo fino a che il grado corrente non è uguale al grado max.
-coloring phase: una volta ordinati i vertici in base ai loro pesi (calcolati al passo precedente), per ogni vertice assegno un coloro nello stesso modo dell'algoritmo LDF.
Infine correggo la colorazione dei vertici errati.

# Luby
Algoritmo **Luby**, Creo un grafo non colorato di comodo, per ogni thread eseguo le seguenti istruzioni:
ogni thread prende la sua parte di grafo su cui dovrà lavorare (come abbiamo visto in precedenza), in questa parte del grafo viene dichiarato un insieme di vertici scelti
casualmente (sfruttando una distribuzione di bernoulli), in questo modo vengono scelti casualmente i vertici del set da inserire nel sotto-sotto-set.
Questo viene fatto finchè tutto il sotto-set non viene inserito casualmente in un altro contenitore (una sorta di shuffle dipendente dalla formula di Luby).
Dopo di che si passa al calcolo del MIS (Maximal Independent set) che viene effettuato come segue: 
Per ogni vertice verifico che non sia stato eliminato, se no allora aggiungo quel vertice ad un vettore V.
Dopodiche scorro quel vettore V e ad ogni iterazione seleziono un vertice e rimuovo un ramo.

# Alcuni Benchamrk
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
