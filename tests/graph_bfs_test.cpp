//
// Created by Mateusz Miliutin on 17/05/2026.
//

#include "../include/ListGraph.hpp"
#include "../include/ReportMaker.hpp"
#include "../include/HelperFunctions.hpp"
#include "../include/BFS.hpp"

#include <format>
#include <iostream>

#define N 10



void try_adding_edge(BaseGraph<int> *graph, const Edge<int>& edge, ReportMaker* r = nullptr) {
    try {
        graph->add_edge( edge );
    }
    catch (std::exception& e) {
        if ( r != nullptr ) r->silent_suberror("Couldn't add edge " + str(edge.source) + "->" + str(edge.target) + ": " + e.what() );
    }
}

void try_generate_random_edges(BaseGraph<int>* graph, ReportMaker* report = nullptr) {
    const int edges_to_add = rand_int(N) + (N);

    for (int i = 0; i < edges_to_add; ++i) {
        const int source = graph->random_node();
        const int target = graph->random_node();
        try_adding_edge(graph,  Edge<int>(source, target, round_float( rand_float(), 2 )), report);
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

// TODO improve this function
template < HasNodes Graph >
void complete_graph(Graph* graph, ReportMaker* r = nullptr) {
    bool flag = true;
    while ( flag ) {
        flag = false;
        for ( const auto source : graph->nodes() ) {
            int size = 0;
            for ([[maybe_unused]] auto edge : graph->adjacents(source)) ++size;

            if ( size == 0 ) {flag = true;}
            while ( true ) {
                try {
                    graph->add_edge(source, graph->random_node(), rand_float());
                    break;
                }
                catch ( EdgeExistsException &ee ) {}
                catch ( std::runtime_error &ne ) {} // TODO add custom exception for loops!!!
            }
        }
    }
}


void generate_random_nodes(ListGraph<int> *graph) {
    while ( graph->v() != N ) {
        try {
            graph->add_node( rand_int( 100 ) );
        } catch ( [[maybe_unused]] NodeExistsException &e ) {}
    }
}


void main_test(ListGraph<int>* graph) {
    auto r = ReportMaker("raport");
    r.header("BFS for graphs");



    auto algorithm = BFS<int, ListGraph<int>>( *graph );

    r.run_test("Generate a connected graph (N="+str(N)+")");
    r.log("Generating a random connected graph");

    generate_random_nodes(graph);
    complete_graph(graph);

    r.log("Saving graph to json");
    graph->get_json("json/bfs_graph.json");

    r.log("Displaying generated graph");
    r.silent_suberror("dislpay() uses std::cout and cannot be saved to report log");
    graph->display();
    r.log( std::format( "Generated graph has {} nodes and {} edges", graph->v(), graph->e() ) );

    r.run_test("Running BFS algorithm");
    int random_node = graph->random_node();
    r.log( std::format( "Picked a random root node for BFS: {}", random_node ) );

    algorithm.run( random_node );

    r.log("BFS display():");
    r.silent_suberror("BFS.display() uses std::cout and cannot be written to report log");
    algorithm.display();

    r.title_line( "END" );
}

int main() {
    rand_init();

    std::vector< ListGraph<int>* > graphs;
    graphs.push_back(new ListGraph<int>(false));

    for (const auto graph : graphs) {
        main_test(graph);
    }

    return 0;
}
