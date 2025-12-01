#ifndef GRAFOBASE_H
#define GRAFOBASE_H

#include <string>
#include <vector>

class GrafoBase {
public:
    virtual ~GrafoBase() {}
    virtual void cargarDatos(std::string archivo) = 0;
    virtual std::vector<int> BFS(int inicio, int profundidad) = 0;
    virtual int obtenerGrado(int nodo) = 0;
    virtual int getNumNodos() = 0;
    virtual int getNumAristas() = 0;
};

#endif // GRAFOBASE_H
