//
// Created by Mateusz M on 28/05/2026.
//
#include "../include/Dijkstra.hpp"
#include "../include/GraphTestUtil.hpp"
#include "../include/ReportMaker.hpp"


constexpr int N = 10;

int main() {
    rand_init(42069);
    const auto r = new ReportMaker("report", "dijkstra");
    const auto graph = new ListGraph<int>();
    const auto algorithm = new Dijkstra<int, ListGraph<int>>(*graph);

    r->header("Finding shortest paths from one node using Dijkstra algorithm");
    r->run_test("Generating a test graph");
    generate_random_nodes(N, graph);
    generate_random_edges(2*N, graph, nullptr);
    r->log("Generated graph:");
    graph->display();
    r->silent_suberror("Graph display() requires std::cout for output and can't be logged");
    r->sublog("Saving generated graph to json");
    graph->get_json("json", "dijkstra_graph.json"); // FIXME make json extension optional

    r->run_test("Dijkstra algorithm");
    r->log("Selecting a random pivot node");
    auto pivot = *graph->node_begin();
    r->sublog("Selected node: "+str(pivot));
    r->log("Running Dijkstra algorithm from the pivot node");
    algorithm->run( *(graph->node_begin()) );
    r->log("Algorithm completed, results:");
    for (auto& pair : algorithm->distance)
        // std::cout << pair.first << " distance : " << pair.second << std::endl;
        r->sublog(str(pair.first) + " distance: " + str(pair.second));

    r->title_line("END");

    delete r;
    delete graph;
    delete algorithm;
    return 0;
}