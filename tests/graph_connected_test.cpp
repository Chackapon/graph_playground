//
// Created by Mateusz M on 21/05/2026.
//


#include <set>

#include "../include/ConnectedComponents.hpp"
#include "../include/GraphTestUtil.hpp"
#include "../include/ListGraph.hpp"
#include "../include/ReportMaker.hpp"

int main() {

    rand_init();

    const auto r = new ReportMaker("report", "is_graph_connected");

    r->header("Is graph connected?");



    r->run_test( "Generating graphs from disconnected node groups" );
    int n_groups = rand_int(2, 5);
    r->log( "Generating " + str(n_groups) + " node groups" );
    std::set<int> groups[n_groups]; // FIXME variable array length is prohibited in C++ ISO
    for (int i = 0; i < n_groups; ++i) {
        for (int j = 0; j < rand_int(3, 7); ++j) groups[i].insert( rand_int(15) );
    }
    r->sublog( "Generated groups: " );
    for ( const auto& group : groups) r->sublog( str(group) );

    r->log("Creating graphs from groups");
    std::list< ListGraph<int>* > temp_graphs;
    int idx = 0;
    for ( const auto& group : groups) {
        temp_graphs.push_back( new ListGraph(group) );
        // temp_graphs.back()->display();
    }
    r->log("Generating edges for those graphs");
    for (const auto graph : temp_graphs) {
        generate_connected_graph( graph );
        graph->get_json("json", "GraphChunk"+str(++idx)+".json");
        // graph->display();
    }



    r->run_test("Summing previous graphs into one");
    ListGraph<int> graph(false);
    for (const auto temp_graph : temp_graphs) {
        // graph = ( graph + *temp_graph ); TODO deosnt work, fix!
        for (const auto node : temp_graph->nodes()) {
            try { graph.add_node(node); } catch( NodeExistsException& ) {} // TODO change all unused exceptions to this
        }
        for (const auto edge : temp_graph->edges()) {
            try { graph.add_edge(*edge); } catch( GraphException& ) {}
        }
    }
    r->log("Flattened temporary graph chunks into one that has "+str(graph.v())+" nodes and "+str(graph.e())+" edges");
    graph.display();
    r->sublog("Exporting summed graph to json");
    graph.get_json("json", "SummedGraph.json");


    r->run_test( "Connected components of the graph" );
    // r->log("Checking if generated graph is disconnected");
    // r->sublog("Result: "+yn()); // WIP
    const auto algorithm = new ConnectedComponents<int, ListGraph<int>>(graph);
    algorithm->run();
    r->log("Running the algorithm");
    r->sublog("Found "+str(algorithm->ncc)+" connected components");


    r->title_line("END");

    // delete graph;
    delete algorithm;
}
