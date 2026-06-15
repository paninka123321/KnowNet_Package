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


void test_search_functions() {
    Graph* g = create_graph(4);
    add_vertex(g, 0, "Ania", EMPLOYEE, "AI, Python");
    add_vertex(g, 1, "Maciek", EMPLOYEE, "Data Science");
    add_vertex(g, 2, "IT_Dept", DEPARTMENT, "");
    add_vertex(g, 3, "Kasia", EMPLOYEE, "AI, HR");

    add_edge(g, 0, 2, 5); // Ania is in IT dep

    printf("\n searching for specialists...\n");

    // Specialist of AI
    int count = 0;
    NodeInfo* ai_experts = find_specialists(g, "AI", &count);
    assert(count == 2);
    printf("we found %d ekspert(s) in AI field:\n", count);
    for (int i = 0; i < count; i++) {
        printf(" -> %s (ID: %d)\n", ai_experts[i].name, ai_experts[i].id);
    }
    free(ai_experts);

    // we are looking for specialists in "Kosmici" (UFOs) - we expect to find none, and the function should handle this gracefully
    NodeInfo* ufo_experts = find_specialists(g, "Kosmici", &count);
    assert(count == 0);
    assert(ufo_experts == NULL);
    printf("We haven't found specialist at Kosmici .\n");

    // employees in IT department (id = 2)
    NodeInfo* it_staff = get_department_employees(g, 2, &count);
    assert(count == 1);
    printf("In it works: %s (ID: %d)\n", it_staff[0].name, it_staff[0].id);
    free(it_staff);

    // what if we try to get employees for Ania (id 0) who is not a department? We expect to get an error message and NULL result.
    printf("looking for employees in the Ania dep \n");
    NodeInfo* error_staff = get_department_employees(g, 0, &count);
    assert(count == 0);
    assert(error_staff == NULL);

    free_graph(g);
    printf("OK: test_search_functions successfuly.\n");
}

void test_cosine_similarity() {
    Graph* g = create_graph(4);
    add_vertex(g, 0, "Alice", EMPLOYEE, "");
    add_vertex(g, 1, "Bob", EMPLOYEE, "");
    add_vertex(g, 2, "Charlie", EMPLOYEE, "");
    add_vertex(g, 3, "Common_Partner", EMPLOYEE, "");

    // Alice & Bob have "Common_Partner"
    add_edge(g, 0, 3, 10);
    add_edge(g, 1, 3, 10);

    double similarity = get_cosine_similarity(g, 0, 1);
    
    printf("\n COSINE SIMILARITY TEST: \n");
    printf("Similarity between Alice and Bob: %f\n", similarity);
    
    assert(similarity > 0.0); // they should habe similarity > 0
    assert(get_cosine_similarity(g, 0, 0) == 1.0); // for sure the similarity to itself should be 1 (the highest)
    
    free_graph(g);
    printf("OK: test_cosine_similarity successfuly.\n");
}

int main() {
    printf("=== Starting tests in C...\n");
    test_centrality_and_busiest();
    test_collaborators();
    test_shortest_path();
    test_search_functions();
    printf("...Test are successful :) ===\n");
    return 0;
}