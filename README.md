# KnowNet - package for Organizational Knowledge Network Analysis 🌐

**Student Name:** Paulina Kulczyk
**Student ID:** 305817


**Keywords:** organizational structure, graph representation, recomendation systems 

## Project Description
**KnowNet** is a high-performance Python package powered by a custom C backend that allows organizations to analyze their internal communication and collaboration networks. By representing employees and departments as a weighted, undirected graph, KnowNet identifies key experts, communication hubs, and optimal collaboration pathways.

## Implemented Functionality & Challenges

I successfully implemented features that answer crucial business questions:
* **Who is the most central person in the firm?** (Degree Centrality)
* **Who does a specific employee collaborate with the most?** (Weighted relationships)
* **Who is the busiest entity?** (Traffic analysis)
* **Who are the closest experts in a specific field?** (Cosine Similarity)

### Sample code & output
```bash
from knownet.core import KnowNetGraph

g = KnowNetGraph(5)

# add vertex
g.add_vertex(0, "Alice", "EMPLOYEE", "AI")
g.add_vertex(1, "Bob", "EMPLOYEE", "AI")
g.add_vertex(3, "IT_DEPT", "DEPARTMENT")

# add edges
g.add_edge(0, 3, 10)  # Alice is connected to IT
g.add_edge(1, 3, 10)  # Bob is connected to IT

# Find specialists
experts = g.get_specialists("AI")
print(f"Found: {experts}")

# Calculate similarity
similarity = g.calculate_similarity("Alice", "Bob")
print(f"Similarity between Alice and Bob: {similarity}")


# Output: 
# Found: [{'id': 0, 'name': 'Alice'}]
# Similarity between Alice and Bob: 1.0
```
## Installation
Ensure you have `gcc` (or another C compiler) and Python 3.8+ installed.

```bash
git clone [repo_link]
cd KnowNet
pip install pytest cython setuptools
pip install -e .
```

## Future Plans
If this project is developed further, I plan to:
1. Introduce dynamic memory reallocation (`realloc`) in C to allow the graph to grow infinitely without specifying the size upfront.
2. Add support for entering non-cse-sensitive string variables to the function
3. Add resistance to string variable endings
4. Introduce more characteristics to vertex; such as: number of raports, position at organization etc.

