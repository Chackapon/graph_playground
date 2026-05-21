#include <iostream>
#include <sstream>

#include "../include/MatrixGraph.hpp"
#include "../include/ListGraph.hpp"
#include "../include/HelperFunctions.hpp"
#include "../include/ReportMaker.hpp"
#include "../include/GraphTestUtil.hpp"

constexpr int N = 10;



void testGraph(BaseGraph<int>* graph) {
    ReportMaker report("report", "GraphTest");
    std::ostringstream graph_id;
    graph_id << graph->graph_implementation() << "@" << graph;


    report.header( "Testing \"" + graph->graph_implementation() + "\" at address " + str((void *)graph) );
    report.log( "Is graph directed? " + yn(graph->is_directed()) );


    report.run_test("add_node(), add_edge() and display()");
    std::vector<int> generated_nodes; // TODO replace with nodes()
    report.log("Testing add_node()");
    try_generate_random_nodes(N, graph, &report, &generated_nodes);
    report.sublog( "Generated nodes: " + str(generated_nodes) + " (plus additionaly 0 and " + str(N-1) + " for testing purposes" );



    report.log( "Testing add_edge()" );
    try_generate_random_edges( N, graph, &report );
    try_adding_edge( Edge<int>(0, N-1, rand_int(N)), graph, &report);
    try_adding_edge( Edge<int>(N-1, 0, rand_int(N)), graph, &report);

    report.sublog( "Generated graph has " + str(graph->v()) + " nodes and " + str(graph->e()) + " edges" );

    report.silent_log("Exporting to json");
    graph->get_json( "json", graph_id.str()+"_before_operations.json" );
    report.line();


    report.log("Testing display()");
    report.silent_suberror("display() uses std::cout and can't be logged. Refer to the terminal for the output result");
    graph->display();


    report.run_test("degree(), indegree(), outdegree() and del_node()");
    report.log( "Selecting random node" );
    int deletion_node = graph->random_node();
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
        source = graph->random_node();
        target = graph->random_node();
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
    graph->get_json( "json", graph_id.str()+".json" );
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
