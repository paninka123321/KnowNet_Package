#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> // special library for testing 
#include "graph.h"

// 1. Testowanie alokacji i zwalniania pamięci
void test_create_and_free_graph() {
    Graph* g = create_graph(5);
    assert(g != NULL);
    assert(g->num_vertices == 5);
    assert(g->vertices != NULL);
    assert(g->adjacency_lists != NULL);
    
    // Sprawdzamy czy listy są na pewno puste po inicjalizacji
    for(int i = 0; i < 5; i++) {
        assert(g->adjacency_lists[i] == NULL);
    }
    
    free_graph(g);
    printf("OK: test_create_and_free_graph przeszlo pomyslnie.\n");
}

// 2. Testowanie dodawania wierzcholków (pracowników/departamentów)
void test_add_vertex() {
    Graph* g = create_graph(2);
    
    add_vertex(g, 0, "Ania", EMPLOYEE, "AI");
    add_vertex(g, 1, "Koszty", DEPARTMENT, NULL);

    // Sprawdzanie poprawnosci danych Ani
    assert(strcmp(g->vertices[0].name, "Ania") == 0);
    assert(g->vertices[0].type == EMPLOYEE);
    assert(strcmp(g->vertices[0].specialist_at, "AI") == 0);

    // Sprawdzanie poprawnosci danych Departamentu
    assert(strcmp(g->vertices[1].name, "Koszty") == 0);
    assert(g->vertices[1].type == DEPARTMENT);
    assert(g->vertices[1].specialist_at[0] == '\0'); // Pusty string

    free_graph(g);
    printf("OK: test_add_vertex przeszlo pomyslnie.\n");
}

// 3. Testowanie dodawania krawedzi i aktualizacji wag
void test_add_edge_and_update() {
    Graph* g = create_graph(3);
    
    // Testowanie dodania nowej relacji
    add_edge(g, 0, 1, 5); // Polaczenie miedzy 0 a 1 o wadze 5
    assert(g->adjacency_lists[0]->destination == 1);
    assert(g->adjacency_lists[0]->weight == 5);
    assert(g->adjacency_lists[1]->destination == 0);
    assert(g->adjacency_lists[1]->weight == 5);

    // Testowanie aktualizacji wagi (wspolpracuja ze soba po raz kolejny)
    add_edge(g, 0, 1, 3); // Dodajemy +3 do wagi
    assert(g->adjacency_lists[0]->weight == 8); // 5 + 3 = 8
    assert(g->adjacency_lists[1]->weight == 8);

    // Testowanie zabezpieczenia przed "self-loop" (praca z samym soba)
    add_edge(g, 2, 2, 10);
    assert(g->adjacency_lists[2] == NULL); // Lista dla ID 2 nadal powinna byc pusta

    free_graph(g);
    printf("OK: test_add_edge_and_update przeszlo pomyslnie.\n");
}

// 4. Testowanie wyszukiwania ID na podstawie imienia
void test_get_id_by_name() {
    Graph* g = create_graph(2);
    add_vertex(g, 0, "Ania", EMPLOYEE, "AI");
    add_vertex(g, 1, "Marketing", DEPARTMENT, "");

    assert(get_id_by_name(g, "Ania") == 0);
    assert(get_id_by_name(g, "Marketing") == 1);
    assert(get_id_by_name(g, "Maciek") == -1); // Brak osoby w grafie

    free_graph(g);
    printf("OK: test_get_id_by_name przeszlo pomyslnie.\n");
}

int main() {
    printf("=== ROZPOCZYNAM TESTY JEDNOSTKOWE (C) ===\n");
    
    test_create_and_free_graph();
    test_add_vertex();
    test_add_edge_and_update();
    test_get_id_by_name();
    
    printf("=== WSZYSTKIE TESTY PRZESZLY POMYSLNIE! ===\n");
    return 0;
}