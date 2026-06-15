#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // to not iterate through whole char array when copying strings
typedef enum {
    DEPARTMENT,
    EMPLOYEE
} NodeType; // here we can define the type of node we need (in our case DEPARTMENT or EMPLOYEE as we present organization structure)

typedef struct {
    int id;
    char name[100];
    NodeType type;
    char specialist_at[150]; // Only for EMPLOYEE type
} Vertex;

typedef struct Edge {
    int destination; // id of the destination vertex
    int weight; // For example, how often employees interact
    struct Edge* next;
} Edge; // a single edge extending from a vertex

typedef struct {
    int num_vertices;
    Vertex* vertices;
    Edge** adjacency_lists; //good for social network graphs where not all vertices are connected to each other
} Graph;


// Function to create a new graph
Graph* create_graph(int num_vertices);

// Funtion to free graph
void free_graph(Graph* graph);

// Function to add a vertex to the graph
void add_vertex(Graph* graph, int id, const char* name, NodeType type, const char* specialist_at);

//  Function to add an edge to the graph
void add_edge(Graph* graph, int src, int dest, int weight);


// Function to print the graph (for testing purposes)
void print_graph(Graph* graph);

// Function that maps name to id
int get_id_by_name(Graph* graph, const char* name);

#endif