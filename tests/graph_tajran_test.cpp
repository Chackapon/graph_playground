//
// Created by Mateusz M on 19/05/2026.
//
#include <iostream>

#include "../include/BaseGraph.hpp"
#include "../include/ReportMaker.hpp"
#include "../include/HelperFunctions.hpp"
#include "../include/ListGraph.hpp"
#include "../include/Tarjan.hpp"

constexpr int N = 10;

void try_adding_edge(BaseGraph<int> *graph, const Edge<int>& edge, ReportMaker* r = nullptr) {
    try {
        graph->add_edge( edge );
    }
    catch (std::exception& e) {
        if ( r != nullptr ) r->silent_suberror("Couldn't add edge " + str(edge.source) + "->" + str(edge.target) + ": " + e.what() );
    }
}

void try_generate_random_nodes(BaseGraph<int>* graph, const int n, ReportMaker* report = nullptr, std::vector<int>* generated = nullptr) {
    for (int i = 0; i < n; ++i) { //add nodes with random values
        const int node = rand_int(n);
        try {
            graph->add_node( node );
            if (generated != nullptr) generated->push_back( node ); //won't be neede with iterators
        } catch (const std::exception& e) {
            if (report != nullptr) report->silent_suberror("Couldn't add node "+str(node)+": "+e.what());
        }

    }
}


template < HasNodes Graph >
void fill_edges(Graph* graph, ReportMaker* r = nullptr) {
    for ( const auto source : graph->nodes() ) {
        for ( const auto target : graph->nodes() ) {
            try_adding_edge(graph, Edge<int>(source, target, rand_float()), r);
        }
    }
}

template < typename T, HasNodes Graph >
bool is_topologically_sorted(Graph* graph, std::vector<T> sorted_nodes) {
    std::unordered_map<T,int> idx;
    for (int i = 0; i < graph->v(); ++i ) {
        idx[sorted_nodes[i]] = i;
    }
    for (auto edge : graph->edges() ) {
        if ( not idx[edge->source] < idx[edge->target] ) return false;
    }
    return true;
}

template < HasNodes Graph >
void generate_dag(Graph* graph, [[maybe_unused]] ReportMaker* r = nullptr) {
    std::vector<int> sorted_nodes;
    std::copy(graph->nodes().begin(), graph->nodes().end(), std::back_inserter(sorted_nodes));
    std::sort(sorted_nodes.begin(), sorted_nodes.end(), [&](int a, int b) { return a < b; }); // NOTE probably optional, check if works without it
    std::cout << str(sorted_nodes) << std::endl;

    for (size_t i = 0; i < sorted_nodes.size(); ++i) {
        for (size_t j = i + 1; j < sorted_nodes.size(); ++j) {
            if (coinflip()) {
                auto new_edge = Edge<int>(sorted_nodes[i], sorted_nodes[j], rand_float());
                try_adding_edge(graph, new_edge);
                std::cout << "Added edge" << new_edge << std::endl;
            }
        }
    }
}



int main () {
    const auto graph = new ListGraph<int>(true);
    const auto r = new ReportMaker("report", "topological_sorting"); // TODO either report should need extension in name or json doesnt
    r->header("Topological sorting using DFS");

    r->run_test("Generating a random DAG graph");
    try_generate_random_nodes(graph, N);
    r->log("Generated a graph with "+str(graph->v())+" nodes");
    r->log("Generating edges");
    generate_dag(graph);
    r->sublog("Generated " + str(graph->e()) + " edges");
    r->log("Displaying graph");
    r->silent_suberror("display() uses std::cout and can't be displayed in a log");
    graph->display();
    r->silent_log("Generating a json file");
    graph->get_json("json", "tarjan.json");

    r->run_test("Running Tarjan topological sorting");
    r->log("Running the sorting algorithm");
    auto sorted_nodes = topsort_kahn<int, ListGraph<int>>( *graph );
    r->sublog("Sorting result: " + str(sorted_nodes) );
    r->sublog("Is this a valid topological sorting of this graph? " + yn(is_topologically_sorted(graph, sorted_nodes) ) );

    r->title_line("END");

    delete graph;
}