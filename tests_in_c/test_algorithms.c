#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "algorithms.h"

// Funkcja pomocnicza budująca graf do testów algorytmów
Graph* build_test_graph() {
    Graph* g = create_graph(5);
    add_vertex(g, 0, "Ania", EMPLOYEE, "");
    add_vertex(g, 1, "Maciek", EMPLOYEE, "");
    add_vertex(g, 2, "Kasia", EMPLOYEE, "");
    add_vertex(g, 3, "IT", DEPARTMENT, "");
    add_vertex(g, 4, "HR", DEPARTMENT, "");

    add_edge(g, 0, 1, 10);
    add_edge(g, 0, 3, 5);
    add_edge(g, 2, 4, 8);
    return g;
}

void test_centrality_and_busiest() {
    Graph* g = build_test_graph();

    // The most central employee is Ania (id 0) because she has the most connections (to Maciek and IT), while Maciek and Kasia have only one connection each, and departments have fewer connections.
    assert(get_most_central_object(g, -1) == 0); 
    // The most central department is IT (id 3) because it has one connection to Ania, while HR has none
    assert(get_busiest_object(g, -1) == 0);

    free_graph(g);
    printf("OK: test_centrality_and_busiest\n");
}

void test_collaborators() {
    Graph* g = build_test_graph();

    // Best collaborator for Ania Maciek id 1
    assert(get_best_collaborator(g, 0, -1) == 1);
    
    // Best collaborator for Ania among departments is IT (id 3)
    assert(get_best_collaborator(g, 0, DEPARTMENT) == 3);

    free_graph(g);
    printf("OK: test_collaborators\n");
}

void test_shortest_path() {
    Graph* g = build_test_graph();
    
    // Dodatkowe połączenie do testów ścieżki
    add_edge(g, 1, 2, 5); // Maciek <-> Kasia

    PathResult res_bfs = get_bfs_shortest_path(g, 0, 2); // Ania -> Kasia
    assert(res_bfs.metric == 2);
    free(res_bfs.path);

    PathResult res_dij = get_dijkstra_shortest_path(g, 0, 2);
    assert(res_dij.metric == 5); // 5 (Maciek<->Kasia)
    free(res_dij.path);

    free_graph(g);
    printf("OK: test_shortest_path\n");
}

int main() {
    printf("=== ROZPOCZYNAM TESTY ALGORYTMOW (C) ===\n");
    test_centrality_and_busiest();
    test_collaborators();
    test_shortest_path();
    printf("=== WSZYSTKIE TESTY ALGORYTMOW PRZESZLY POMYSLNIE! ===\n");
    return 0;
}