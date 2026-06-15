#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h> 
#endif
#include "algorithms.h"

double get_cosine_similarity(Graph* graph, int id_a, int id_b) {
    if (graph == NULL || id_a < 0 || id_a >= graph->num_vertices || id_b < 0 || id_b >= graph->num_vertices) {
        printf("Error! Invalid input to get_cosine_similarity function.\n");
        return -1.0;
    }
    if (id_a == id_b) return 1.0;

    int V = graph->num_vertices;
    double* vec_a = (double*)calloc(V, sizeof(double));
    double* vec_b = (double*)calloc(V, sizeof(double));

    
    Edge* curr = graph->adjacency_lists[id_a];
    while (curr != NULL) { vec_a[curr->destination] = (double)curr->weight; curr = curr->next; }
    curr = graph->adjacency_lists[id_b];
    while (curr != NULL) { vec_b[curr->destination] = (double)curr->weight; curr = curr->next; }

    double dot_product = 0.0;
    double norm_a = 0.0;
    double norm_b = 0.0;

    // Parallelize the loop for summation with OpenMP 
    #ifdef _OPENMP
        #pragma omp parallel for reduction(+:dot_product, norm_a, norm_b)  
    #endif
    for (int i = 0; i < V; i++) {
        dot_product += vec_a[i] * vec_b[i];
        norm_a += vec_a[i] * vec_a[i];
        norm_b += vec_b[i] * vec_b[i];
    }

    free(vec_a);
    free(vec_b);

    if (norm_a == 0.0 || norm_b == 0.0) return 0.0;
    return dot_product / (sqrt(norm_a) * sqrt(norm_b));
}