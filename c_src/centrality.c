#include <stdio.h>
#include "algorithms.h"

// object with the most connections (degree centrality)
// target_type: 0 (DEPARTMENT), 1 (EMPLOYEE) or -1 (all)
int get_most_central_object(Graph* graph, int target_type) {
    if (graph == NULL || graph->num_vertices == 0){
        printf("Error! Graph is NULL or has no vertices.\n");
        return -1;
    }

    //check if type is available in our graph:
    if (target_type != -1 && target_type != DEPARTMENT && target_type != EMPLOYEE) {
        printf("Error! Invalid target_type: %d. Use -1 (ALL), 0 (DEPARTMENT) or 1 (EMPLOYEE).\n", target_type);
        return -1;
    }

    int max_connections = -1; // Initialize to -1 to ensure any vertex with at least 0 connections will be considered
    int central_id = -1; // Initialize to -1 to indicate no valid vertex found yet

    for (int i = 0; i < graph->num_vertices; i++) {
        if (target_type != -1 && graph->vertices[i].type != target_type) continue; 

        int current_connections = 0;
        Edge* current = graph->adjacency_lists[i];
        
        while (current != NULL) {
            current_connections++;
            current = current->next;
        }

        if (current_connections > max_connections) {
            max_connections = current_connections;
            central_id = i;
        }
    }
    return central_id;
}

// object with the most interactions (weight sum)
// target_type: 0 (DEPARTMENT), 1 (EMPLOYEE) or -1 (all)
int get_busiest_object(Graph* graph, int target_type) {
    if (graph == NULL || graph->num_vertices == 0){
        printf("Error! Graph is NULL or has no vertices.\n");
        return -1;
    }

    if (target_type != -1 && target_type != DEPARTMENT && target_type != EMPLOYEE) {
        printf("Error! Invalid target_type: %d. Use -1 (ALL), 0 (DEPARTMENT) or 1 (EMPLOYEE).\n", target_type);
        return -1;
    }

    int max_weight_sum = -1;
    int busiest_id = -1;

    for (int i = 0; i < graph->num_vertices; i++) {
        if (target_type != -1 && graph->vertices[i].type != target_type) continue; 

        int current_weight_sum = 0;
        Edge* current = graph->adjacency_lists[i];
        
        while (current != NULL) {
            current_weight_sum += current->weight; // we count total weight of interactions for this vertex
            current = current->next;
        }

        if (current_weight_sum > max_weight_sum) {
            max_weight_sum = current_weight_sum;
            busiest_id = i;
        }
    }
    return busiest_id;
}