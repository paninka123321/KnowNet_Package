from libc.stdlib cimport malloc, free

cdef extern from "graph.h":
    ctypedef enum NodeType:
        DEPARTMENT = 0
        EMPLOYEE = 1

    ctypedef struct Graph:
        pass

    Graph* create_graph(int num_vertices)
    void free_graph(Graph* graph)
    void add_vertex(Graph* graph, int id, const char* name, NodeType type, const char* specialist_at)
    void add_edge(Graph* graph, int src, int dest, int weight)
    void print_graph(Graph* graph)
    int get_id_by_name(Graph* graph, const char* name)

cdef extern from "algorithms.h":
    int get_most_central_object(Graph* graph, int target_type)
    int get_busiest_object(Graph* graph, int target_type)
    int get_best_collaborator(Graph* graph, int person_id, int target_type)

cdef class KnowNetGraph:
    cdef Graph* _c_graph 

    def __cinit__(self, int num_vertices):
        self._c_graph = create_graph(num_vertices)
        if self._c_graph is NULL:
            raise MemoryError("Nie udało się zaalokować pamięci dla grafu.")

    def __dealloc__(self):
        if self._c_graph is not NULL:
            free_graph(self._c_graph)

    def add_vertex(self, int vertex_id, str name, str node_type, str specialist_at=""):
        cdef NodeType c_type
        if node_type.upper() == "DEPARTMENT":
            c_type = DEPARTMENT
        elif node_type.upper() == "EMPLOYEE":
            c_type = EMPLOYEE
        else:
            raise ValueError("Type must be 'DEPARTMENT' or 'EMPLOYEE'")

        cdef bytes py_bytes_name = name.encode('utf-8')
        cdef const char* c_name = py_bytes_name

        cdef bytes py_bytes_spec
        cdef const char* c_spec = NULL  
        
        if specialist_at:
            py_bytes_spec = specialist_at.encode('utf-8')
            c_spec = py_bytes_spec

        add_vertex(self._c_graph, vertex_id, c_name, c_type, c_spec)

    def add_edge(self, int src, int dest, int weight=1):
        add_edge(self._c_graph, src, dest, weight)

    def show(self):
        print_graph(self._c_graph)

    def get_id(self, str name):
        cdef bytes py_bytes_name = name.encode('utf-8')
        return get_id_by_name(self._c_graph, py_bytes_name)

    cdef int _parse_type(self, str target_type):
        t = target_type.upper()
        if t == "ALL": return -1
        if t == "DEPARTMENT": return 0
        if t == "EMPLOYEE": return 1
        raise ValueError("target_type must be 'ALL', 'DEPARTMENT', or 'EMPLOYEE'")

    def most_central(self, str target_type="ALL"):
        return get_most_central_object(self._c_graph, self._parse_type(target_type))

    def busiest(self, str target_type="ALL"):
        return get_busiest_object(self._c_graph, self._parse_type(target_type))

    def best_collaborator(self, int person_id, str target_type="ALL"):
        return get_best_collaborator(self._c_graph, person_id, self._parse_type(target_type))