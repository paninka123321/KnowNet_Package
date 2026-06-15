#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> // special library for testing
#include "graph.h"

// we start from testing memory management
void test_create_and_free_graph() {
    Graph* g = create_graph(5);
    assert(g != NULL);
    assert(g->num_vertices == 5);
    assert(g->vertices != NULL);
    assert(g->adjacency_lists != NULL);
    
    // Initially, all adjacency lists should be NULL (no edges yet)
    for(int i = 0; i < 5; i++) {
        assert(g->adjacency_lists[i] == NULL);
    }

    // Now we free the graph and check if it doesn't crash (we can't easily check memory leaks here, but at least we can ensure it doesn't segfault)
    free_graph(g);
    printf("OK: test_create_and_free_graph created graph successfully.\n");
}

// Tests of adding vertices and ensuring their properties are set correctly
void test_add_vertex() {
    Graph* g = create_graph(2);
    
    add_vertex(g, 0, "Ania", EMPLOYEE, "AI");
    add_vertex(g, 1, "Costs", DEPARTMENT, NULL); // here we test that NULL is handled properly for specialist_at

    assert(strcmp(g->vertices[0].name, "Ania") == 0);
    assert(g->vertices[0].type == EMPLOYEE);
    assert(strcmp(g->vertices[0].specialist_at, "AI") == 0);

    assert(strcmp(g->vertices[1].name, "Costs") == 0);
    assert(g->vertices[1].type == DEPARTMENT);
    // we ensure that if there is no specialist_at for a DEPARTMENT, it should be an empty string 
    assert(g->vertices[1].specialist_at[0] == '\0'); 

    free_graph(g);
    printf("OK: test_add_vertex successfully.\n");
}

// Test for edges
void test_add_edge_and_update() {
    Graph* g = create_graph(3);
    
    add_edge(g, 0, 1, 5); 
    assert(g->adjacency_lists[0]->destination == 1);
    assert(g->adjacency_lists[0]->weight == 5);
    assert(g->adjacency_lists[1]->destination == 0);
    assert(g->adjacency_lists[1]->weight == 5);

    // with new colaborator, the weight should update to 8 (5 + 3)
    add_edge(g, 0, 1, 3); 
    assert(g->adjacency_lists[0]->weight == 8); 
    assert(g->adjacency_lists[1]->weight == 8);

    // Adding an edge to a non-existent vertex should not crash and should not add an edge
    add_edge(g, 2, 2, 10);
    assert(g->adjacency_lists[2] == NULL); // empty in this case

    free_graph(g);
    printf("OK: test_add_edge_and_update successfully.\n");
}

// Test for get_id_by_name function
void test_get_id_by_name() {
    Graph* g = create_graph(2);
    add_vertex(g, 0, "Ania", EMPLOYEE, "AI");
    add_vertex(g, 1, "Marketing", DEPARTMENT, "");

    assert(get_id_by_name(g, "Ania") == 0);
    assert(get_id_by_name(g, "Marketing") == 1);
    assert(get_id_by_name(g, "Maciek") == -1); // Nie ma Maćka, funkcja musi zwrócić -1

    free_graph(g);
    printf("OK: test_get_id_by_name successfully.\n");
}

int main() {
    printf("=== Test of graph structure started:\n");
    
    test_create_and_free_graph();
    test_add_vertex();
    test_add_edge_and_update();
    test_get_id_by_name();
    
    printf("All tests are successfully :) ===\n");
    return 0;
}