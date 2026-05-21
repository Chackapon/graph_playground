//
// Created by Mateusz M on 17/05/2026.
//

#include <format>
#include <iostream>

#include "../include/ListGraph.hpp"
#include "../include/ReportMaker.hpp"
#include "../include/HelperFunctions.hpp"
#include "../include/BFS.hpp"
#include "../include/GraphTestUtil.hpp"

#define N 10



void main_test(ListGraph<int>* graph) {
    auto r = ReportMaker("report", "bfs_test");
    r.header("BFS for graphs");



    auto algorithm = BFS<int, ListGraph<int>>( *graph );

    r.run_test("Generate a connected graph (N="+str(N)+")");
    r.log("Generating a random connected graph");

    generate_random_nodes(N, graph);
    generate_connected_graph(graph);

    r.log("Saving graph to json");
    graph->get_json("../json", "BFS_graph.json");

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
