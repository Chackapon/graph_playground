//
// Created by Mateusz M on 20/05/2026.
//

#ifndef ALGOSY2GRAFY_GRAPHTESTUTIN_HPP
#define ALGOSY2GRAFY_GRAPHTESTUTIN_HPP

#include <unordered_map>

#include "BaseGraph.hpp"
#include "GraphExceptions.hpp"
#include "HelperFunctions.hpp"
#include "ListGraph.hpp"
#include "ReportMaker.hpp"

//region ===================== NODE GENERATION =====================
inline void try_generate_random_nodes(const int N, BaseGraph<int>* graph, ReportMaker* report = nullptr, std::vector<int>* generated = nullptr) {
    for (int i = 0; i < N; ++i) { //add nodes with random values
        const int node = rand_int(N);
        try {
            graph->add_node( node );
            if (generated != nullptr) generated->push_back( node ); //won't be neede with iterators
        } catch (const std::exception& e) {
            if (report != nullptr) report->silent_suberror("Couldn't add node "+str(node)+": "+e.what());
        }

    }
}
// TODO add ability to generate any label
inline void generate_random_nodes(const int N, BaseGraph<int> *graph) {
    while ( graph->v() != N ) {
        try {
            graph->add_node( rand_int( 100 ) ); // TODO parametrize this
        } catch ( [[maybe_unused]] NodeExistsException &e ) {}
    }
}
//endregion

//region ===================== EDGE GENERATION =====================
/**
 * Used to correctly handle exceptions when trying to add an edge to a graph by logging that exception to a report log
 * @param edge An Edge object
 * @param graph Pointer to Graph
 * @param report Pointer to an optional ReportMaker object for debugging purposes (leave blank if you just want to skip the exceptions)
 */
inline void try_adding_edge(const Edge<int>& edge, BaseGraph<int> *graph, ReportMaker* report = nullptr) {
    try {
        graph->add_edge( edge );
    }
    catch (GraphException& e) {
        if ( report != nullptr ) report->silent_suberror("Couldn't add edge " + str(edge.source) + "->" + str(edge.target) + ": " + e.what() );
    }
    catch ( [[maybe_unused]] std::runtime_error& re ) { // TODO replace with Loop error
        if ( report != nullptr ) report->silent_suberror("Couldn't add edge " + str(edge.source) + "->" + str(edge.target) + ": " + re.what() );
    }
}
inline void try_generate_random_edges(const int N, BaseGraph<int>* graph, ReportMaker* report) {
    const int edges_to_add = rand_int(N) + N; // TODO improve this

    for (int i = 0; i < edges_to_add; ++i) {
        const int source = graph->random_node();
        const int target = graph->random_node();
        try_adding_edge(Edge(source, target, round_float( rand_float(), 2 )), graph, report);
    }
}
inline void generate_random_edges(const int N, BaseGraph<int> *graph, ReportMaker* report) {
    while ( graph->e() != N ) { // FIXME remove constants
        const int source = graph->random_node();
        const int target = graph->random_node();
        try_adding_edge( Edge( source, target, round_float( rand_float(), 2 ) ), graph, report ); // TODO parametrize value function
    }
}
template < HasNodes Graph >
void fill_edges(Graph* graph, ReportMaker* r = nullptr) {
    for ( const auto source : graph->nodes() ) {
        for ( const auto target : graph->nodes() ) {
            try_adding_edge(Edge<int>(source, target, rand_float()), graph, r);
        }
    }
}
template < HasNodes Graph >
void generate_connected_graph(Graph* graph, ReportMaker* r = nullptr) {
    bool flag = true;
    while ( flag ) {
        flag = false;
        for ( const auto source : graph->nodes() ) {

            int size = 0;
            for ([[maybe_unused]] auto edge : graph->adjacents(source)) ++size;
            // std::cout << source << ": " << size << std::endl;
            if ( size == 0 ) {flag = true;}
            while ( flag ) {
                try {
                    graph->add_edge(source, graph->random_node(), rand_float());
                    break;
                }
                catch ( [[maybe_unused]] EdgeExistsException &ee ) {}
                catch ( [[maybe_unused]] std::runtime_error &ne ) {} // TODO add custom exception for loops!!!
            }
        }
    }
}
template < HasNodes Graph >
void generate_dag(Graph* graph, ReportMaker* r = nullptr) {
    std::vector<int> sorted_nodes;
    std::copy(graph->nodes().begin(), graph->nodes().end(), std::back_inserter(sorted_nodes));
    std::sort(sorted_nodes.begin(), sorted_nodes.end(), [&](int a, int b) { return a < b; }); // NOTE probably optional, check if works without it

    for (size_t i = 0; i < sorted_nodes.size(); ++i) {
        for (size_t j = i + 1; j < sorted_nodes.size(); ++j) {
            if (coinflip()) {
                auto new_edge = Edge<int>(sorted_nodes[i], sorted_nodes[j], rand_float());
                try_adding_edge(new_edge, graph, r);
            }
        }
    }
}
//endregion

//region ===================== GRAPH PROPERTIES =====================
template < typename T, HasNodes Graph >
bool is_topologically_sorted(Graph* graph, std::vector<T> test_nodes) {
    std::unordered_map<T,int> idx;
    for (int i = 0; i < graph->v(); ++i ) {
        idx[test_nodes[i]] = i;
    }
    for (auto edge : graph->edges() ) {
        if ( idx[edge->source] >= idx[edge->target] ) return false;
    }
    return true;
}
//endregion

#endif //ALGOSY2GRAFY_GRAPHTESTUTIN_HPP
