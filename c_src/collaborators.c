#include <stdio.h>
#include "algorithms.h"

// the most frequent collaborator for a given person (the one with the highest weight of interaction)
int get_best_collaborator(Graph* graph, int person_id, int target_type) {

    if (graph == NULL || person_id < 0 || person_id >= graph->num_vertices) {
        printf("Error! Graph is NULL or person_id is out of range.\n");
        return -1;
    }
    if (target_type != -1 && target_type != DEPARTMENT && target_type != EMPLOYEE) {
        printf("Error! Invalid target_type: %d. Use -1 (ALL), 0 (DEPARTMENT) or 1 (EMPLOYEE).\n", target_type);
        return -1;
    }

    int max_weight = -1;
    int best_collaborator_id = -1;
    
    Edge* current = graph->adjacency_lists[person_id];
    
    while (current != NULL) {
        int dest_id = current->destination;
        NodeType dest_type = graph->vertices[dest_id].type;

        // check if the destination vertex matches the target type (or if we want to consider all types)
        if (target_type == -1 || dest_type == target_type) {
            if (current->weight > max_weight) {
                max_weight = current->weight;
                best_collaborator_id = dest_id;
            }
        }
        current = current->next;
    }
    
    return best_collaborator_id; 
}