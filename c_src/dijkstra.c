#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "algorithms.h"

// Modified Dijkstra: Maximum Bottleneck Path + Minimal Hops.
// Finds the path with the strongest communication links. If tied, picks the shortest path.
PathResult get_dijkstra_shortest_path(Graph* graph, int start_id, int end_id) {
    PathResult result = {NULL, 0, -1}; // Default "not found" state
    
    if (graph == NULL) {
        printf("Error! The graph doesn't exist. I can't search empty space.\n");
        return result;
    }

    if (start_id < 0 || start_id >= graph->num_vertices) {
        printf("Error! Start_id %d is out of ranges (not exist in graph).\n", start_id);
        return result;
    }

    if (end_id < 0 || end_id >= graph->num_vertices) {
        printf("Error! End_id %d is out of ranges (not exist in graph).\n", end_id);
        return result;
    }

    if (start_id == end_id) { // If start and end are the same, the path is just that vertex with 0 hops.
        result.metric = 0; 
        result.path_length = 0; 
        result.path = NULL; 
        return result;
    }

    int total_people = graph->num_vertices;
    
    int* max_capacities = (int*)calloc(total_people, sizeof(int)); 
    int* visited_nodes = (int*)calloc(total_people, sizeof(int));
    int* previous_person = (int*)malloc(total_people * sizeof(int));
    
    // we also count how many hops we did to optimize to not hop thprough 100 people if we can hop through 2
    int* hops = (int*)malloc(total_people * sizeof(int));

    if (max_capacities == NULL || visited_nodes == NULL || previous_person == NULL || hops == NULL) {
        printf("OOM: Out of memory.\n");
        if (max_capacities) free(max_capacities);
        if (visited_nodes) free(visited_nodes);
        if (previous_person) free(previous_person);
        if (hops) free(hops);
        return result;
    }

    // Initialize all nodes as unvisited, with 0 capacity and no previous person, and infinite hops (we want to minimize hops, so we start with a very high number)
    for (int i = 0; i < total_people; i++) {
        previous_person[i] = -1;
        hops[i] = INT_MAX; 
    }

    int INFINITY_CAPACITY = INT_MAX;
    max_capacities[start_id] = INFINITY_CAPACITY;
    hops[start_id] = 0; //hops to ourself is 0

    // main loop
    for (int step = 0; step < total_people; step++) {
        
        int current_max_capacity = -1;
        int current_min_hops = INT_MAX;
        int best_node_to_visit = -1;

        // Find the unvisited node with the highest max capacity. If there's a tie, pick the one with fewer hops.
        for (int v = 0; v < total_people; v++) {
            if (visited_nodes[v] == 0) {
                // th first priority is the strongest path to this node
                if (max_capacities[v] > current_max_capacity) {
                    current_max_capacity = max_capacities[v];
                    current_min_hops = hops[v];
                    best_node_to_visit = v;
                } // the second priority is the number of hops if the capacity is the same
                else if (max_capacities[v] == current_max_capacity && hops[v] < current_min_hops) {
                    current_min_hops = hops[v];
                    best_node_to_visit = v;
                }
            }
        }

        if (best_node_to_visit == -1 || current_max_capacity == 0) break;

        visited_nodes[best_node_to_visit] = 1;

        Edge* current_edge = graph->adjacency_lists[best_node_to_visit];
        while (current_edge != NULL) {
            int neighbor_id = current_edge->destination;
            int communication_strength = current_edge->weight;

            int path_bottleneck = (max_capacities[best_node_to_visit] < communication_strength) 
                                  ? max_capacities[best_node_to_visit] 
                                  : communication_strength;
            
            // how many hops would it take to get to this neighbor through the best_node_to_visit?
            int potential_hops = hops[best_node_to_visit] + 1;

            if (visited_nodes[neighbor_id] == 0) {
                // we found a stronger path to this neighbor! Let's update it.
                if (path_bottleneck > max_capacities[neighbor_id]) {
                    max_capacities[neighbor_id] = path_bottleneck;
                    hops[neighbor_id] = potential_hops;
                    previous_person[neighbor_id] = best_node_to_visit;
                }
                // if we found a path with the same bottleneck capacity but fewer hops, we also want to update it (because we want to minimize hops as a tiebreaker)
                else if (path_bottleneck == max_capacities[neighbor_id] && potential_hops < hops[neighbor_id]) {
                    hops[neighbor_id] = potential_hops;
                    previous_person[neighbor_id] = best_node_to_visit;
                }
            }
            
            current_edge = current_edge->next;
        }
    }

    
    // results 
    if (max_capacities[end_id] > 0) {
        result.metric = max_capacities[end_id];
        
        int curr = end_id;
        int len = 0;
        while (curr != -1) {
            len++;
            curr = previous_person[curr];
        }
        
        result.path_length = len;
        result.path = (int*)malloc(len * sizeof(int));
        
        curr = end_id;
        for (int i = len - 1; i >= 0; i--) {
            result.path[i] = curr;
            curr = previous_person[curr];
        }
    }

    // free allocated memory
    free(max_capacities);
    free(visited_nodes);
    free(previous_person);
    free(hops); 

    return result;
}