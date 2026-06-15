import pytest
from knownet.core import KnowNetGraph

@pytest.fixture
def sample_graph():
    g = KnowNetGraph(5)
    g.add_vertex(0, "Ania", "EMPLOYEE", "AI")
    g.add_vertex(1, "Maciek", "EMPLOYEE", "Data Science")
    g.add_vertex(2, "Kasia", "EMPLOYEE", "HR")
    g.add_vertex(3, "IT_Dept", "DEPARTMENT")
    g.add_vertex(4, "HR_Dept", "DEPARTMENT")
    
    g.add_edge(0, 1, 10) 
    g.add_edge(0, 3, 5)  
    g.add_edge(2, 4, 8)  
    return g

# --- TESTY: CENTRALITY & BUSIEST ---
def test_most_central(sample_graph):
    assert sample_graph.most_central("ALL") == 0
    assert sample_graph.most_central("DEPARTMENT") in [3, 4]

def test_busiest(sample_graph):
    assert sample_graph.busiest("ALL") == 0
    assert sample_graph.busiest("EMPLOYEE") == 0

# --- TESTY: COLLABORATORS ---
def test_best_collaborator(sample_graph):
    assert sample_graph.best_collaborator(0, "ALL") == 1
    assert sample_graph.best_collaborator(0, "DEPARTMENT") == 3

def test_invalid_algorithm_type(sample_graph):
    with pytest.raises(ValueError):
        sample_graph.most_central("WRONG_TYPE")

# --- TESTY: PATHFINDING ---
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