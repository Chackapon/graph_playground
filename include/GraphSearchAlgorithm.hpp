//
// Created by Mateusz M on 19/05/2026.
//

#ifndef ALGOSY2GRAFY_GRAPHSEARCHALGORITHM_HPP
#define ALGOSY2GRAFY_GRAPHSEARCHALGORITHM_HPP

#include <unordered_map>
#include <vector>

#include "BaseGraph.hpp"

enum search_mode {
    PREORDER, POSTORDER
};

template <typename T, typename G> // node type, graph type
requires Graph<G, T>
class GraphSearchAlgorithm {
    protected:
    G& graph;
    std::unordered_map<T, bool> visited;
    public:
    virtual ~GraphSearchAlgorithm() = default;
    explicit GraphSearchAlgorithm( G& g ) : graph(g) {
    } ;

    std::vector<T> preorder;
    std::vector<T> postorder;
    std::unordered_map<T, T> parent; // drzewo DFS

    virtual void run(T u) { visit(u); }
    virtual void run() {
        for ( auto n_it : graph.nodes() ) {
            if (!visited[n_it])
                visit(n_it);
        }
    }
    virtual void visit(T u, int lvl = 0) = 0;
    virtual void display(search_mode mode = PREORDER) = 0;
};


#endif //ALGOSY2GRAFY_GRAPHSEARCHALGORITHM_HPP