#include "GrafoDisperso.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <map>
#include <set>

GrafoDisperso::GrafoDisperso() : num_nodos(0), num_aristas(0) {}

GrafoDisperso::~GrafoDisperso() {}

void GrafoDisperso::cargarDatos(std::string archivo) {
    std::cout << "[C++ Core] Loading dataset '" << archivo << "'..." << std::endl;
    std::ifstream file(archivo);
    if (!file.is_open()) {
        std::cerr << "[C++ Core] Error: Could not open file " << archivo << std::endl;
        return;
    }

    // First pass: Count nodes and edges, and build adjacency list temporarily
    // Note: For truly massive graphs where adjacency list might be too big, 
    // we would need a more streaming approach or 2-pass construction. 
    // Given 8-16GB RAM constraint and ~1M nodes, a temporary vector of vectors is risky but likely okay 
    // if vectors are carefully managed. 
    // However, to be strictly CSR from the start is harder without knowing degrees.
    // Let's use a map of vectors first to handle non-contiguous IDs if any, 
    // then compact to CSR. Or better, just read max node ID first.
    
    // Assuming node IDs are integers. SNAP datasets usually are.
    // We will use a temporary adjacency list structure: vector<vector<int>>.
    // To handle potentially large gaps in node IDs, we might need to map IDs, 
    // but for standard SNAP datasets (like web-Google), IDs are usually dense enough or we just resize.
    
    // Let's assume 0-based dense indexing for simplicity or max_id sizing.
    
    int u, v;
    int max_id = 0;
    std::vector<std::pair<int, int>> edges;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        if (ss >> u >> v) {
            edges.push_back({u, v});
            max_id = std::max(max_id, std::max(u, v));
        }
    }
    file.close();

    num_nodos = max_id + 1;
    num_aristas = edges.size();
    
    std::cout << "[C++ Core] Max Node ID: " << max_id << ", Edges: " << num_aristas << std::endl;

    // Sort edges by source, then destination to build CSR easily
    std::sort(edges.begin(), edges.end());

    // Build CSR
    row_ptr.assign(num_nodos + 1, 0);
    col_indices.reserve(num_aristas);
    values.assign(num_aristas, 1); // Unweighted

    int current_row = 0;
    int edge_count = 0;

    for (const auto& edge : edges) {
        int src = edge.first;
        int dst = edge.second;

        // Fill row_ptr for rows with no edges or moving to next row
        while (current_row < src) {
            current_row++;
            row_ptr[current_row] = edge_count;
        }
        
        // If we are at the correct row (which we should be), add edge
        col_indices.push_back(dst);
        edge_count++;
    }
    
    // Fill remaining row_ptrs
    while (current_row < num_nodos) {
        current_row++;
        row_ptr[current_row] = edge_count;
    }

    std::cout << "[C++ Core] CSR structure built. Memory estimated: " 
              << (row_ptr.size() * 4 + col_indices.size() * 4) / (1024.0 * 1024.0) << " MB." << std::endl;
}

int GrafoDisperso::obtenerGrado(int nodo) {
    if (nodo < 0 || nodo >= num_nodos) return 0;
    return row_ptr[nodo + 1] - row_ptr[nodo];
}

std::vector<int> GrafoDisperso::getVecinos(int nodo) {
    std::vector<int> vecinos;
    if (nodo < 0 || nodo >= num_nodos) return vecinos;

    int start = row_ptr[nodo];
    int end = row_ptr[nodo + 1];

    for (int i = start; i < end; ++i) {
        vecinos.push_back(col_indices[i]);
    }
    return vecinos;
}

std::vector<int> GrafoDisperso::BFS(int inicio, int profundidad) {
    std::vector<int> visitados;
    if (inicio < 0 || inicio >= num_nodos) return visitados;

    std::queue<std::pair<int, int>> q; // node, depth
    std::set<int> visited_set;

    q.push({inicio, 0});
    visited_set.insert(inicio);
    visitados.push_back(inicio);

    while (!q.empty()) {
        int u = q.front().first;
        int d = q.front().second;
        q.pop();

        if (d >= profundidad) continue;

        int start = row_ptr[u];
        int end = row_ptr[u + 1];

        for (int i = start; i < end; ++i) {
            int v = col_indices[i];
            if (visited_set.find(v) == visited_set.end()) {
                visited_set.insert(v);
                visitados.push_back(v);
                q.push({v, d + 1});
            }
        }
    }
    return visitados;
}

int GrafoDisperso::getNumNodos() {
    return num_nodos;
}

int GrafoDisperso::getNumAristas() {
    return num_aristas;
}
