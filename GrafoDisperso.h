#ifndef GRAFODISPERSO_H
#define GRAFODISPERSO_H

#include "GrafoBase.h"
#include <vector>
#include <string>
#include <iostream>

class GrafoDisperso : public GrafoBase {
private:
    int num_nodos;
    int num_aristas;
    
    // CSR Format
    std::vector<int> values;       // Always 1 for unweighted graphs, but kept for structure
    std::vector<int> col_indices;  // Column indices of edges
    std::vector<int> row_ptr;      // Row pointers

public:
    GrafoDisperso();
    ~GrafoDisperso();

    void cargarDatos(std::string archivo) override;
    std::vector<int> BFS(int inicio, int profundidad) override;
    int obtenerGrado(int nodo) override;
    int getNumNodos() override;
    int getNumAristas() override;
    
    // Helper to get neighbors for visualization
    std::vector<int> getVecinos(int nodo);
};

#endif // GRAFODISPERSO_H
