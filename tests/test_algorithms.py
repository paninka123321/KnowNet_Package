import pytest
from knownet.core import KnowNetGraph

@pytest.fixture
def sample_graph():
    g = KnowNetGraph(10)
    g.add_vertex(0, "Ania", "EMPLOYEE", "AI")
    g.add_vertex(1, "Maciek", "EMPLOYEE", "Data Science")
    g.add_vertex(2, "Kasia", "EMPLOYEE", "HR")
    g.add_vertex(3, "IT_Dept", "DEPARTMENT")
    g.add_vertex(4, "HR_Dept", "DEPARTMENT")
    
    g.add_edge(0, 1, 10) 
    g.add_edge(0, 3, 5)  
    g.add_edge(2, 4, 8)  
    return g

# TESTS: CENTRALITY & BUSIEST 
def test_most_central(sample_graph):
    assert sample_graph.most_central("ALL") == 0
    assert sample_graph.most_central("DEPARTMENT") in [3, 4]

def test_busiest(sample_graph):
    assert sample_graph.busiest("ALL") == 0
    assert sample_graph.busiest("EMPLOYEE") == 0

# TESTS: COLLABORATORS 
def test_best_collaborator(sample_graph):
    assert sample_graph.best_collaborator(0, "ALL") == 1
    assert sample_graph.best_collaborator(0, "DEPARTMENT") == 3

def test_invalid_algorithm_type(sample_graph):
    with pytest.raises(ValueError):
        sample_graph.most_central("WRONG_TYPE")

# TESTS: PATHFINDING 
def test_shortest_path_direct(sample_graph):
    res_bfs = sample_graph.shortest_path("Ania", "Maciek", method="bfs")
    assert res_bfs["metric"] == 1
    
    res_dij = sample_graph.shortest_path("Ania", "Maciek", method="dijkstra")
    assert res_dij["metric"] == 10

def test_shortest_path_self(sample_graph):
    res = sample_graph.shortest_path("Ania", "Ania", method="bfs")
    assert res["metric"] == 0

def test_shortest_path_disconnected(sample_graph):
    g = KnowNetGraph(2) 
    g.add_vertex(0, "Ania", "EMPLOYEE", "AI")
    g.add_vertex(1, "Ghost", "EMPLOYEE", "Biology Department")

    res = g.shortest_path("Ania", "Ghost", method="bfs")
    assert res["metric"] == -1
    assert res["path"] == []

def test_invalid_path_method(sample_graph):
    with pytest.raises(ValueError):
        sample_graph.shortest_path("Ania", "Maciek", method="WRONG_METHOD")

@pytest.fixture
def tie_breaker_graph():
    g = KnowNetGraph(5)
    g.add_vertex(0, "Start", "EMPLOYEE")
    g.add_vertex(1, "track1", "EMPLOYEE")
    g.add_vertex(2, "track1b", "EMPLOYEE")
    g.add_vertex(3, "target", "EMPLOYEE")
    g.add_vertex(4, "track2", "EMPLOYEE")

    # track 1 1: Start -> Track1 -> Track1b -> target (3 hops, bottleneck= 10)
    g.add_edge(0, 1, 10)
    g.add_edge(1, 2, 10)
    g.add_edge(2, 3, 10)

    # Trasa 2: Start -> track2 -> target (2 hops, wąskie bottleneck = 10)
    g.add_edge(0, 4, 10)
    g.add_edge(4, 3, 10)
    
    return g

def test_dijkstra_tie_breaker(tie_breaker_graph):
    res = tie_breaker_graph.shortest_path("Start", "target", method="dijkstra")
    
    assert res["metric"] == 10
    assert res["path"] == [0, 4, 3]


# TESTS: searching for specialist functions
def test_get_specialists(sample_graph):
    eksperci = sample_graph.get_specialists("AI")
    assert len(eksperci) == 1
    assert eksperci[0]["name"] == "Ania"
    assert eksperci[0]["id"] == 0
    print(f"\nWe found sepcialist at AI: {eksperci[0]}")

def test_get_specialists_empty(sample_graph, capsys):
    eksperci = sample_graph.get_specialists("Kosmici")
    assert eksperci == []
    
    captured = capsys.readouterr()
    assert "There are no specialist" in captured.out
    print("\n[We try to find 'Kosmici'.")

def test_get_department_employees(sample_graph):
    it_staff = sample_graph.get_department_employees("IT_Dept")
    assert len(it_staff) == 1
    assert it_staff[0]["name"] == "Ania"
    print(f"\nEmployees in IT dep: {it_staff}")

def test_get_department_error(sample_graph):
    with pytest.raises(ValueError) as exc_info:
        sample_graph.get_department_employees("LackDep")
    
    print(f"\nThere is no LackDep department with error: '{exc_info.value}'")

# TESTS: cosine similarity
def test_cosine_similarity(sample_graph):
    # we add collaborators for Ania - Tomek
    sample_graph.add_vertex(5, "Tomek", "EMPLOYEE")
    sample_graph.add_edge(5, 3, 10)
    
    sim = sample_graph.calculate_similarity("Ania", "Tomek")
    assert sim > 0  # for this workers we should have some similarity
    print(f"\n[PYTHON WYNIK] Podobieństwo Ani i Tomka: {sim}")

def test_cosine_similarity_calculation(sample_graph):
    sample_graph.add_vertex(5, "Alice", "EMPLOYEE")
    sample_graph.add_vertex(6, "Bob", "EMPLOYEE")
    sample_graph.add_edge(5, 3, 10) 
    sample_graph.add_edge(6, 3, 10)
    
    sim = sample_graph.calculate_similarity("Alice", "Bob")
    
    assert sim == 1.0
    print(f"\n[Similarity between Alice and Bob: {sim}")

def test_cosine_similarity_no_relation(sample_graph):
    # now lets add someone who has no contacts
    sample_graph.add_vertex(7, "Loner", "EMPLOYEE")
    
    sim = sample_graph.calculate_similarity("Ania", "Loner")
    
    assert sim == 0.0
    print(f"\nSimilarity between Ania and Loner: {sim}")