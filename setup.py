from setuptools import setup, Extension
from Cython.Build import cythonize

# Definiujemy nasze rozszerzenie napisane w C/Cython
extensions = [
    Extension(
        name="knownet.core",                 # from knownet.core import...
        sources=[
            "knownet/core.pyx",              
            "c_src/graph.c",
            "c_src/centrality.c",
            "c_src/collaborators.c",
            "c_src/bfs.c",            
            "c_src/dijkstra.c"
        ],
        include_dirs=["c_src"]               # for .h files
    )
]

setup(
    name="knownet",                          # My package name :)
    packages=["knownet"],                    
    ext_modules=cythonize(extensions, language_level="3")
)