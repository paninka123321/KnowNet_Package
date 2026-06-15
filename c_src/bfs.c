#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "algorithms.h"

// BFS: Finds the shortest path in terms of "hops" (ignoring weights).
// Returns the number of hops, or -1 if they are not connected.
PathResult get_bfs_shortest_path(Graph* graph, int start_id, int end_id) {
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

    // If start and end are the same, the path is just that vertex with 0 hops.
    if (start_id == end_id) {
        result.metric = 0;
        result.path_length = 0; // No path needed if it's the same person!
        result.path = NULL;
        return result; 
    }

    int total_people = graph->num_vertices;
    int* queue = (int*)malloc(total_people * sizeof(int)); 
    int* visited = (int*)calloc(total_people, sizeof(int)); 
    int* distances = (int*)calloc(total_people, sizeof(int));
    
    // NEW: Keep track of who introduced us to this node (breadcrumbs for the path)
    int* previous_person = (int*)malloc(total_people * sizeof(int));

    if (queue == NULL || visited == NULL || distances == NULL || previous_person == NULL) {
        printf("OOM: Out of RAM!\n");
        if (queue) free(queue);
        if (visited) free(visited);
        if (distances) free(distances);
        if (previous_person) free(previous_person);
        return result;
    }

    // Initialize previous_person with -1 (meaning "no one")
    for (int i = 0; i < total_people; i++) previous_person[i] = -1;

    int queue_front = 0, queue_back = 0;
    queue[queue_back++] = start_id; 
    visited[start_id] = 1; 
    distances[start_id] = 0; 

    // The main BFS loop
    while (queue_front < queue_back) {
        int current_person = queue[queue_front++];

        if (current_person == end_id) {
            result.metric = distances[current_person];
            break; // Found it!
        }

        Edge* current_edge = graph->adjacency_lists[current_person];
        while (current_edge != NULL) {
            int neighbor_id = current_edge->destination;

            if (visited[neighbor_id] == 0) {
                visited[neighbor_id] = 1; 
                distances[neighbor_id] = distances[current_person] + 1;
                previous_person[neighbor_id] = current_person; // !mark!
                queue[queue_back++] = neighbor_id;
            }
            current_edge = current_edge->next;
        }
    }

    // If we found a path, let's reconstruct it using the !mark!
    if (result.metric != -1) {
        // calculate path length
        int curr = end_id;
        int len = 0;
        while (curr != -1) {
            len++;
            curr = previous_person[curr];
        }
        
        result.path_length = len;
        result.path = (int*)malloc(len * sizeof(int));
        
        // Trace back and put it in the array (in reverse order so it reads start -> end)
        curr = end_id;
        for (int i = len - 1; i >= 0; i--) {
            result.path[i] = curr;
            curr = previous_person[curr];
        }
    }

    // Clean up
    free(queue);
    free(visited);
    free(distances);
    free(previous_person);

    return result;
}