# boost_vector_as_graph_with_property

This header defines some boost traits for `vector<vector< pair<size_t, WeightType> >>` (list of outgoing edges with weights for each vertex) in order to allow use it as graph for some boost distance algorithms (such as Dijkstra algorithm).

You can see an example of usage in `main.cpp`.
