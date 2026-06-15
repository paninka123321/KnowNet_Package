#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graph.h" // we need graph structure

// look at centrality.c
int get_most_central_object(Graph* graph, int target_type);
int get_busiest_object(Graph* graph, int target_type);

// look at collaborators.c
int get_best_collaborator(Graph* graph, int person_id, int target_type);

#endif