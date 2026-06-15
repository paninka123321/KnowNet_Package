#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graph.h" // we need graph structure

typedef struct {
    int* path;        // Array of vertex IDs representing the path
    int path_length;  // Number of vertices in the path
    int metric;       // The metric value (e.g., number of hops for BFS, total weight for Dijkstra)
} PathResult;

// look at centrality.c
int get_most_central_object(Graph* graph, int target_type);
int get_busiest_object(Graph* graph, int target_type);

// look at collaborators.c
int get_best_collaborator(Graph* graph, int person_id, int target_type);

// look at bfs.c
PathResult get_bfs_shortest_path(Graph* graph, int start_id, int end_id);

// look at dijkstra.c
PathResult get_dijkstra_shortest_path(Graph* graph, int start_id, int end_id);

#endif