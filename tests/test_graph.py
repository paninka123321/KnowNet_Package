import pytest
from knownet.core import KnowNetGraph

@pytest.fixture
def sample_graph():
    """ Tests the basic graph initialization and vertex addition. """
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

def test_graph_initialization(sample_graph):
    """Testuje, czy graf poprawnie przypisuje ID na podstawie imion/nazw."""
    assert sample_graph.get_id("Ania") == 0
    assert sample_graph.get_id("IT_Dept") == 3
    assert sample_graph.get_id("BrakOsoby") == -1

def test_invalid_vertex_type(sample_graph):
    """Testuje zabezpieczenie przed zlym typem przy dodawaniu wierzcholka."""
    with pytest.raises(ValueError):
        sample_graph.add_vertex(10, "unknown", "error_type")