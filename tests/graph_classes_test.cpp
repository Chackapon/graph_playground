#include <iostream>
#include <sstream>

#include "../include/MatrixGraph.hpp"
#include "../include/ListGraph.hpp"
#include "../include/HelperFunctions.hpp"
#include "../include/ReportMaker.hpp"

constexpr int N = 10;

void try_adding_edge(BaseGraph<int> *graph, const Edge<int>& edge, ReportMaker& r) {
    try {
        graph->add_edge( edge );
    }
    catch (std::exception& e) {
        r.silent_suberror("Couldn't add edge " + str(edge.source) + "->" + str(edge.target) + ": " + e.what() );
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

void try_generate_random_edges(BaseGraph<int>* graph, ReportMaker* report, std::vector<int>* generated_nodes) {
    const int edges_to_add = rand_int(N) + (N);

    for (int i = 0; i < edges_to_add; ++i) {
        // add random edges
        const int source = generated_nodes->at( rand_int( (int)generated_nodes->size() ) );
        const int target = generated_nodes->at( rand_int( (int)generated_nodes->size() ) );
        try_adding_edge(graph,  Edge<int>(source, target, round_float( rand_float(), 2 )), *report);
    }
}

void testGraph(BaseGraph<int>* graph) {
    ReportMaker report("raport");
    std::ostringstream graph_id;
    graph_id << graph->graph_implementation() << "@" << (void*)graph;


    report.header( "Testing \"" + graph->graph_implementation() + "\" at address " + str((void *)graph) );
    report.log( "Is graph directed? " + yn(graph->is_directed()) );


    report.run_test("add_node(), add_edge() and display()");
    std::vector<int> generated_nodes; // zastanowic sie czy nie da sie tego zrobic lepiej!!!
    // std::cout << "* Testing add_node()" << std::endl;
    report.log("Testing add_node()");
    try_generate_random_nodes(graph, N, &report, &generated_nodes);

    report.sublog( "Generated nodes: " + str(generated_nodes) + " (plus additionaly 0 and " + str(N-1) + " for testing purposes" );
    // std::cout << "(this number may not equal to amount of final nodes, ass add_edge() can add the target node if it doesn't exist)" << std::endl;



    report.log( "Testing add_edge()" );
    try_generate_random_edges(graph, &report, &generated_nodes);
    try_adding_edge(graph,  Edge<int>(0, N-1, rand_int(N)), report);
    try_adding_edge(graph,  Edge<int>(N-1, 0, rand_int(N)), report);

    report.sublog( "Generated graph has " + str(graph->v()) + " nodes and " + str(graph->e()) + " edges" );

    report.silent_log("Exporting to json");
    graph->get_json( "json/"+graph_id.str()+"_before_operations.json" );
    report.line();


    report.log("Testing display()");
    report.silent_suberror("display() uses std::cout and can't be logged. Refer to the terminal for the output result");
    graph->display();


    report.run_test("degree(), indegree(), outdegree() and del_node()");
    report.log( "Selecting random node" );
    int deletion_node = generated_nodes[ rand_int( generated_nodes.size() ) ];
    report.log( "Selected node: " + str(deletion_node) );
    report.log( "Degree: " + str(graph->degree( deletion_node )) );
    report.log( "Indegree: " + str(graph->indegree( deletion_node )) );
    report.log( "Outdegree: " + str(graph->outdegree( deletion_node )) );
    report.log( "Is node " + str(deletion_node) + " in the graph? " + yn(graph->has_node(deletion_node)) );
    report.sublog( "Deleting node: " + str(deletion_node) );
    graph->del_node( deletion_node );
    // generated_nodes.erase( std::find(generated_nodes.begin(), generated_nodes.end(), deletion_node) );
    report.log( "Is node " + str(deletion_node) + " in the graph? " + yn(graph->has_node(deletion_node)) );


    report.run_test("has_edge(), weight() and del_edge()");
    report.log("Selecting random pair of nodes");
    int source = 0, target = 0;

    while (true) {
        try {
            if ( graph->has_edge(source, target) ) break;
        } catch (std::exception& e) {
            report.suberror("Couldn't check if edge " + str(source) + "->" + str(target) + " exists: " + e.what());
        }
        source = generated_nodes[ rand_int(generated_nodes.size()) ] ;
        target = generated_nodes[ rand_int(generated_nodes.size()) ] ;
    }

    report.log( "Selected nodes: " + str(source) + " and " + str(target) );
    report.sublog( "Does edge exist? " + yn(graph->has_edge(source, target)) );
    report.sublog("Edges weight: " + str(graph->weight(source, target)) );
    report.log( "Testing del_edge()" );
    graph->del_edge(source, target);
    report.sublog( "Does edge exist? " + yn(graph->has_edge(source, target)) );


    // // Dodanie krawędzi automatycznie dodaje brakujące wierzchołki.
    report.run_test("Does adding edge add lacking nodes?");
    int breaker = 0;
    while (graph->has_node(source)) { source = rand_int(N); if (++breaker>N*N) break; }
    while (graph->has_node(target)) { target = rand_int(N); if (++breaker>N*N) break; }
    report.log( "Selected nodes: " + str(source) + " and " + str(target) );
    report.sublog( "Does node " + str(source) + " exist? " + yn(graph->has_node(source)) );
    report.sublog( "Does node " + str(target) + " exist? " + yn(graph->has_node(target)) );
    report.log("Adding edge");

    try {
        graph->add_edge(source, target);
    } catch (const std::exception& e) {
        report.suberror( "Couldn't add edge " + str(source) + "->" + str(target) + ": " + e.what() );
    }

    // std::cout << "\t> Does edge exist? ";
    // try {
    //      std::cout << yn(graph->has_edge(source, target));
    // } catch (GraphException& e) {
    //     std::cout << e.what();
    // }
    // std::cout << std::endl;
    report.sublog( "Does node " + str(source) + " exist? " + yn(graph->has_node(source)) );
    report.sublog( "Does node " + str(target) + " exist? " + yn(graph->has_node(target)) );

    // std::cout << "\t> Does edge exist? " << yn(graph->has_edge(source, target)) << std::endl;


    report.run_test( "clear()" );
    report.log( "Final graph has " + str(graph->v()) + " nodes and " + str(graph->e()) + " edges" );
    report.silent_log( "Exporting to json" );
    graph->get_json( "json/"+graph_id.str()+".json" );
    report.log( "Testing clear()" );
    graph->clear();
    report.sublog( "After clear() graph has " + str(graph->v()) + " nodes and " + str(graph->e()) + " edges" );
    report.title_line( "END" );
    std::cout << "\n" << std::endl;

}

int main() {
    rand_init();

    std::list< BaseGraph<int>* > graph_collection;
    graph_collection.push_back( new ListGraph<int>(true) );
    graph_collection.push_back( new ListGraph<int>(false) );
    graph_collection.push_back( new MatrixGraph(N, true) );
    graph_collection.push_back( new MatrixGraph(N, false) );


    for (const auto graph : graph_collection) testGraph(graph);


    return 0;
}
