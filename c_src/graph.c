#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "graph.h"

// Function to create a new graph
Graph* create_graph(int num_vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->vertices = (Vertex*)malloc(num_vertices * sizeof(Vertex));
    graph->adjacency_lists = (Edge**)malloc(num_vertices * sizeof(Edge*));
    for (int i = 0; i < num_vertices; i++) {
        graph->adjacency_lists[i] = NULL; // Initialize adjacency lists to NULL
    }
    return graph;
}

// Funtion to free graph
void free_graph(Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        Edge* current = graph->adjacency_lists[i];
        while (current != NULL) {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjacency_lists);
    free(graph->vertices);
    free(graph);
}

// Function to add a vertex to the graph
void add_vertex(Graph* graph, int id, const char* name, NodeType type, const char* specialist_at) {
    // At this stage of project we assume that graph has set size
    if (id < 0 || id >= graph->num_vertices) {
        printf("Error! Vertex of id %d is out of range (max id: %d)\n", id, graph->num_vertices - 1);
        return;
    }

    // v - vertex of our aim
    Vertex* v = &graph->vertices[id];

    v->id = id;
    v->type = type;
    strncpy(v->name, name, sizeof(v->name) - 1);
    v->name[sizeof(v->name) - 1] = '\0'; // to be sure that string is null-terminated

    // specialization, we assume that department doesn't have any
    if (type == EMPLOYEE && specialist_at != NULL) {
        strncpy(v->specialist_at, specialist_at, sizeof(v->specialist_at) - 1);
        v->specialist_at[sizeof(v->specialist_at) - 1] = '\0';
    } else {
        v->specialist_at[0] = '\0'; 
    }
}


//  Function to add an edge to the graph
void add_edge(Graph* graph, int src, int dest, int weight){
    if (src < 0 || src  >= graph->num_vertices) {
        printf("Error! Vertex of id %d is out of range (max id: %d)\n", src, graph->num_vertices - 1);
        return;
    }

    if (dest < 0 || dest  >= graph->num_vertices) {
        printf("Error! Vertex of id %d is out of range (max id: %d)\n", dest, graph->num_vertices - 1);
        return;
    }

    // we assume that one person cannot communcate with themselves, so we don't add edge if src and dest are the same
    if (src == dest) {
        printf("Eror! Person cannot work with themselves %d.", src);
        return; 
    }

    // We assume that if one person is in contact with another, the other person is also in contact with the first
    // So we add edges in both directions = undirected graph

    Edge* current = graph->adjacency_lists[src];
    while (current != NULL) {
        if (current->destination == dest) {
            // Actualization of previous edge - we just increase the weight of existing edge instead of adding new one
            current->weight += weight;
            Edge* reverse = graph->adjacency_lists[dest];
            while (reverse != NULL) {
                if (reverse->destination == src) {
                    reverse->weight += weight;
                    return; 
                }
                reverse = reverse->next;
            }
            return;
        }
        current = current->next;
    }
    
    // new edge src -> dest
    Edge* edge1 = (Edge*)malloc(sizeof(Edge));
    if (edge1 == NULL) {
        printf("Error! Cannot allocate memory for this edge.\n");
        return;
    }
    edge1->destination = dest;
    edge1->weight = weight;
    edge1->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = edge1;

    // new edge dest -> src
    Edge* edge2 = (Edge*)malloc(sizeof(Edge));
    if (edge2 == NULL) {
        printf("Error! Cannot allocate memory for this edge.\n");
        return;
    }
    edge2->destination = src;
    edge2->weight = weight;
    edge2->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = edge2;
}


// Function to print the graph (for testing purposes)
void print_graph(Graph* graph) {
    if (graph == NULL) return;
    
    printf("\n--- Organization Structure ---\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        Vertex* v = &graph->vertices[i];
        
        // Vertex info
        printf("[%d] %s (%s", v->id, v->name, v->type == EMPLOYEE ? "Employee" : "Department");
        if (v->type == EMPLOYEE && v->specialist_at[0] != '\0') {
            printf(", %s", v->specialist_at);
        }
        printf(") works with: ");

        // Neighbors info
        Edge* current = graph->adjacency_lists[i];
        if (current == NULL) {
            printf("No one"); // If there are no neighbors, print "No one"
        }
        while (current != NULL) {
            printf("-> ID:%d (weight:%d) ", current->destination, current->weight);
            current = current->next;
        }
        printf("\n");
    }
    printf("-----------------------------\n");
}

// Function that maps name to id
int get_id_by_name(Graph* graph, const char* name) {
    if (graph == NULL || name == NULL) return -1;

    for (int i = 0; i < graph->num_vertices; i++) {
        if (strcmp(graph->vertices[i].name, name) == 0) {
            return i; 
        }
    }
    return -1; // not found person with this name
}
