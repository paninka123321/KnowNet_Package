#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algorithms.h"


// Function to find specialists based on a given specialty
NodeInfo* find_specialists(Graph* graph, const char* specialty, int* out_count) {
    if (graph == NULL || specialty == NULL || out_count == NULL){
        printf("Error! Invalid input to find_specialists function.\n");
        return NULL;
    }

    *out_count = 0;

    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->vertices[i].specialist_at[0] != '\0' && 
            strstr(graph->vertices[i].specialist_at, specialty) != NULL) {
            (*out_count)++;
        }
    }

    if (*out_count == 0){ 
        printf("No specialists found for the specialty: %s\n", specialty);
        return NULL;
    }

    NodeInfo* results = (NodeInfo*)malloc((*out_count) * sizeof(NodeInfo));
    int index = 0;
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->vertices[i].specialist_at[0] != '\0' && 
            strstr(graph->vertices[i].specialist_at, specialty) != NULL) {
            
            results[index].id = i;
            results[index].name = graph->vertices[i].name; // here we get name of the employee based on id
            index++;
        }
    }

    return results;
}

// Function to get all employees in a specific department
NodeInfo* get_department_employees(Graph* graph, int dept_id, int* out_count) {
    if (graph == NULL || out_count == NULL || dept_id < 0 || dept_id >= graph->num_vertices)
    {
        printf("Error! Invalid input to get_department_employees function.\n");
        return NULL;
    } 
    
    *out_count = 0;

    // we have to check if given id is an id of the depatament
    if (graph->vertices[dept_id].type != DEPARTMENT) {
        printf("Error: This ID %d is not an ID of departament!\n", dept_id);
        return NULL;
    }

    // how much employees work in this departamnet 
    Edge* curr = graph->adjacency_lists[dept_id];
    while (curr != NULL) {
        if (graph->vertices[curr->destination].type == EMPLOYEE) {
            (*out_count)++;
        }
        curr = curr->next;
    }

    if (*out_count == 0) return NULL;

    
    NodeInfo* results = (NodeInfo*)malloc((*out_count) * sizeof(NodeInfo));
    curr = graph->adjacency_lists[dept_id];
    int idx = 0;
    
    while (curr != NULL) {
        int neighbor_id = curr->destination;
        if (graph->vertices[neighbor_id].type == EMPLOYEE) {
            results[idx].id = neighbor_id;
            results[idx].name = graph->vertices[neighbor_id].name;
            idx++;
        }
        curr = curr->next;
    }

    return results;
}