import pytest
from knownet.core import KnowNetGraph

@pytest.fixture
def sample_graph():
    """
    This fixture creates a sample graph for testing purposes. 
    It initializes a KnowNetGraph with 5 vertices, adds some employees and departments, and establishes edges with weights representing the strength of collaboration.
    """
    g = KnowNetGraph(5)
    
    # some basic emplyees and departments
    g.add_vertex(0, "Ania", "EMPLOYEE", "AI")
    g.add_vertex(1, "Maciek", "EMPLOYEE", "Data Science")
    g.add_vertex(2, "Kasia", "EMPLOYEE", "HR")
    g.add_vertex(3, "IT_Dept", "DEPARTMENT")
    g.add_vertex(4, "HR_Dept", "DEPARTMENT")
    
    # edges with weights representing strength of collaboration
    g.add_edge(0, 1, 10) # Ania <-> Maciek 
    g.add_edge(0, 3, 5)  # Ania <-> IT 
    g.add_edge(2, 4, 8)  # Kasia <-> HR 
    
    return g

def test_graph_initialization(sample_graph):
    """Testuje, czy graf poprawnie przypisuje ID na podstawie imion/nazw."""
    assert sample_graph.get_id("Ania") == 0
    assert sample_graph.get_id("IT_Dept") == 3
    assert sample_graph.get_id("BrakOsoby") == -1

def test_most_central(sample_graph):
    """Testuje algorytm szukania najbardziej centralnego obiektu (najwięcej unikalnych połączeń)."""
    # Ania (id = 0) has 2 unique connections (Maciek and IT), while others have fewer, so she should be the most central.
    assert sample_graph.most_central("ALL") == 0
    
    # Among departments, IT (3) and HR (4) both have 1 connection, so either can be returned
    assert sample_graph.most_central("DEPARTMENT") in [3, 4]

def test_busiest(sample_graph):
    """Testuje algorytm szukania najbardziej zajętego obiektu (największa suma wag)."""
    # Ania (id = 0) has a total weight of 15 (10 with Maciek and 5 with IT), while others have less, so she should be the busiest.
    assert sample_graph.busiest("ALL") == 0
    # Among employees, Ania (0) has the highest total weight, so she should be the busiest.
    assert sample_graph.busiest("EMPLOYEE") == 0

def test_best_collaborator(sample_graph):
    """It tests the algorithm for finding the best collaborator (the one with the highest weight of interaction)."""
    # The best collaborator for Ania (0) among all types is Maciek (1) with a weight of 10.
    assert sample_graph.best_collaborator(0, "ALL") == 1
    
    # The best collaborator for Ania (0) among departments is IT (3) with a weight of 5.
    assert sample_graph.best_collaborator(0, "DEPARTMENT") == 3

def test_invalid_type(sample_graph):
    """It tests the behavior of the graph when an invalid type is provided."""
    with pytest.raises(ValueError):
        sample_graph.most_central("WRONG_TYPE")
        
    with pytest.raises(ValueError):
        sample_graph.add_vertex(10, "unknown", "erorr_type")