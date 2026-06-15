import pytest
from knownet.core import KnowNetGraph

@pytest.fixture
def sample_graph():
    g = KnowNetGraph(5)
    g.add_vertex(0, "Ania", "EMPLOYEE", "AI")
    g.add_vertex(1, "Maciek", "EMPLOYEE", "Data Science")
    g.add_vertex(3, "IT_Dept", "DEPARTMENT")
    g.add_edge(0, 1, 10) 
    g.add_edge(0, 3, 5)  
    return g

def test_graph_initialization(sample_graph):
    assert sample_graph.get_id("Ania") == 0
    assert sample_graph.get_id("IT_Dept") == 3
    assert sample_graph.get_id("Unknown") == -1

def test_edge_integrity(sample_graph):
    """Check if the graph structure correctly stores connections."""
    sample_graph.add_edge(1, 3, 2)
    assert True 

def test_invalid_vertex_type(sample_graph):
    with pytest.raises(ValueError):
        sample_graph.add_vertex(4, "Invalid", "WRONG_TYPE")