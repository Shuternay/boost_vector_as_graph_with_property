#include <boost/config.hpp>

#include <iostream>
#include <fstream>
#include <ctime>
#include <limits>
#include <vector>
#include <queue>

#include "vector_as_graph_with_property.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/overloading.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include <boost/concept/assert.hpp>


class Graph {
public:
    size_t getNumberOfVertexes() {
        return edges.size();
    }

    void readGraph(std::istream &in) {
        size_t n, m;
        in >> n >> m;

        edges.resize(n);

        for (size_t i = 0; i < m; ++i) {
            int from, to;
            short weight;
            in >> from >> to >> weight;

            edges[from - 1].push_back({to - 1, weight});
            edges[to - 1].push_back({from - 1, weight});
        }
    }


    // 1-indexation
    template<class T = boost::directedS>
    int findShortestPathBoostDijkstraOnAdjList(size_t from, size_t to) {
        --from;
        --to;

        typedef boost::property<boost::edge_weight_t, int> Weight;
        typedef typename boost::adjacency_list<
                boost::vecS,
                boost::vecS,
                T,
                boost::no_property,
                Weight> MyGraph;
        typedef typename boost::graph_traits<MyGraph>::vertex_descriptor vertex_descriptor;

        MyGraph graph(edges.size());

        clock_t begin_time = clock();
        for (size_t i = 0; i < edges.size(); ++i)
            for (size_t j = 0; j < edges[i].size(); ++j) {
                boost::add_edge(boost::vertex(i, graph),
                                boost::vertex(edges[i][j].first, graph),
                                Weight(edges[i][j].second),
                                graph
                );
                boost::add_edge(boost::vertex(edges[i][j].first, graph),
                                boost::vertex(i, graph),
                                Weight(edges[i][j].second),
                                graph
                );
            }

        auto s = boost::vertex(from, graph);

        std::vector<vertex_descriptor> p(boost::num_vertices(graph));
        std::vector<int> d(boost::num_vertices(graph));

        std::cout << "converting to boost format: " << (clock() - begin_time) * 1.0 / CLOCKS_PER_SEC << "\n";
        begin_time = clock();
        boost::dijkstra_shortest_paths_no_color_map(graph, s, boost::predecessor_map(&p[0]).distance_map(&d[0]));
        std::cout << "boost dijkstra time: " << (clock() - begin_time) * 1.0 / CLOCKS_PER_SEC << "\n";
        return d[to] < edges.size() * 256 ? d[to] : -1;
    }


    // 1-indexation
    int findShortestPathBoostDijkstraOnVector(size_t from, size_t to) {
        --from;
        --to;

        BOOST_CONCEPT_ASSERT(( boost::IncidenceGraphConcept< std::vector<std::vector<std::pair<int, int>>> > ));


        typedef typename boost::graph_traits<std::vector<std::vector<std::pair<int, int>>>>::vertex_descriptor vertex_descriptor;

        auto s = from;

        std::vector<vertex_descriptor> p(boost::num_vertices(edges));
        std::vector<int> d(boost::num_vertices(edges));

        boost::dijkstra_shortest_paths_no_color_map(edges, s, boost::predecessor_map(&p[0]).distance_map(&d[0]));
        return d[to] < edges.size() * 256 ? d[to] : -1;
    }

private:
    std::vector<std::vector<std::pair<int, int> > > edges;

};


int main() {
    Graph g;

    clock_t begin_time;
    begin_time = clock();

    g.readGraph(std::cin);

    std::cout << "reading file: " << (clock() - begin_time) * 1.0 / CLOCKS_PER_SEC << "\n";


    int boostDirDijkstraDist = g.findShortestPathBoostDijkstraOnAdjList<boost::directedS>(1, g.getNumberOfVertexes());

//     int boostBidirDijkstraDist = g.findShortestPathBoostDijkstraOnAdjList<boost::bidirectionalS>(1, g.getNumberOfVertexes());

//    int boostUndirDijkstraDist = g.findShortestPathBoostDijkstraOnAdjList<boost::undirectedS>(1, g.getNumberOfVertexes());


    begin_time = clock();
    int boostMYVDijkstraDist = g.findShortestPathBoostDijkstraOnVector(1, g.getNumberOfVertexes());
    std::cout << "boost vector dijkstra time: " << (clock() - begin_time) * 1.0 / CLOCKS_PER_SEC << "\n";


    if (boostDirDijkstraDist != boostMYVDijkstraDist) {
        std::cout << "WA: "
        << "boost::adjacency_list: " << boostDirDijkstraDist
        << ", boost::vector_as_graph_with_property: " << boostMYVDijkstraDist << "\n\n";
    }
    else {
        std::cout << "OK: " << boostDirDijkstraDist << "\n\n";
    }

    return 0;
}