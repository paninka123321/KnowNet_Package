# KnowNet - package for Organizational Knowledge Network Analysis 🌐

**Student Name:** Paulina Kulczyk
**Student ID:** 305817
**Keywords:** organizational structure, graph representation, recomendation systems 

## Project Description
**KnowNet** is a high-performance Python package powered by a custom C backend that allows organizations to analyze their internal communication and collaboration networks. By representing employees and departments as a weighted, undirected graph, KnowNet identifies key experts, communication hubs, and optimal collaboration pathways.

## Implemented Functionality & Challenges
--

I successfully implemented features that answer crucial business questions:
* **Who is the most central person in the firm?** (Degree Centrality)
* **Who does a specific employee collaborate with the most?** (Weighted relationships)
* **Who is the busiest entity (Employee or Department)?** 

## Future Plans
If this project is developed further, I plan to:
1. Introduce dynamic memory reallocation (`realloc`) in C to allow the graph to grow infinitely without specifying the size upfront.

## Installation
Ensure you have `gcc` (or another C compiler) and Python 3.8+ installed.

```bash
git clone [repo_link]
cd Project
pip install pytest cython setuptools
pip install -e .