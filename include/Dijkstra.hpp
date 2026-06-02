//
// Created by Mateusz M on 22/05/2026.
//

#ifndef ALGOSY2GRAFY_DIJKSTRA_HPP
#define ALGOSY2GRAFY_DIJKSTRA_HPP
#include <format>
#include <cmath>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

#include "HelperFunctions.hpp"


// dijkstra.hpp
template <typename VertexType, typename G> // node type, graph type
class Dijkstra {
    G& graph;
public:
    std::unordered_map<VertexType, VertexType> parent;     // drzewo najkrótszych ścieżek
    std::unordered_map<VertexType, float> distance; // odległość od źródła
    explicit Dijkstra(G& g) : graph(g) {

    }
    ~Dijkstra() = default; // destruktor
    void run( VertexType u ) {
        // Step 1: initialization
        std::set<VertexType> processed_vertices;
        std::priority_queue< std::pair< VertexType, float>, std::vector<std::pair< VertexType, float >>, MapValueComparator<VertexType> > vertice_queue;

        for ( const auto vertice : graph.nodes() ) {
            distance[vertice] = INFINITY;
            parent[vertice] = vertice;
            if ( vertice != u) vertice_queue.push( std::make_pair(vertice, INFINITY) );
        }

        // Special handling for pivot vertex
        distance[u] = 0;
        vertice_queue.push( std::make_pair(u, 0) );

        // Step 2: edge relaxation
        while ( !vertice_queue.empty() ) {
            VertexType v = vertice_queue.top().first;
            vertice_queue.pop();
            processed_vertices.insert(v);

            for ( const auto edge : graph.adjacents(v) ) {
                auto alt = distance[v] + edge->weight;
                if ( alt < distance[edge->target] ) {
                    distance[edge->target] = alt;
                    parent[edge->target] = v;
                }
            }


        }
    }
};
// Usage:
// auto algorithm = Dijkstra<int,Graph>(G); // macierz sąsiedztwa
// auto algorithm = Dijkstra<char,Graph<char>>(G); // lista sąsiedztwa
// algorithm.run(source);
// for (auto& pair : algorithm.distance)
//     std::cout << pair.first << " distance : " << pair.second << std::endl;


#endif //ALGOSY2GRAFY_DIJKSTRA_HPP
