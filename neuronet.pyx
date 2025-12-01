# distutils: language = c++

from libcpp.vector cimport vector
from libcpp.string cimport string
from neuronet cimport GrafoDisperso

cdef class NeuroNetEngine:
    cdef GrafoDisperso* c_grafo  # Hold a pointer to the C++ instance

    def __cinit__(self):
        self.c_grafo = new GrafoDisperso()

    def __dealloc__(self):
        del self.c_grafo

    def load_data(self, str filename):
        self.c_grafo.cargarDatos(filename.encode('utf-8'))

    def bfs(self, int start_node, int depth):
        return self.c_grafo.BFS(start_node, depth)

    def get_degree(self, int node):
        return self.c_grafo.obtenerGrado(node)
    
    def get_num_nodes(self):
        return self.c_grafo.getNumNodos()

    def get_num_edges(self):
        return self.c_grafo.getNumAristas()

    def get_neighbors(self, int node):
        return self.c_grafo.getVecinos(node)
