from libcpp.vector cimport vector
from libcpp.string cimport string

cdef extern from "../cpp/GrafoBase.h":
    cdef cppclass GrafoBase:
        pass

cdef extern from "../cpp/GrafoDisperso.h":
    cdef cppclass GrafoDisperso(GrafoBase):
        GrafoDisperso() except +
        void cargarDatos(string archivo)
        vector[int] BFS(int inicio, int profundidad)
        int obtenerGrado(int nodo)
        int getNumNodos()
        int getNumAristas()
        vector[int] getVecinos(int nodo)
