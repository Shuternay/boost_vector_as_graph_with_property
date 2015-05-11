#ifndef VECTOR_AS_GRAPH_WITH_PROPERTY_VECTOR_AS_GRAPH_WITH_PROPERTY_H
#define VECTOR_AS_GRAPH_WITH_PROPERTY_VECTOR_AS_GRAPH_WITH_PROPERTY_H


#include <boost/config.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/type.hpp>

#include <boost/graph/properties.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/range/irange.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/pending/property.hpp>
#include <vector>

namespace boost {
    struct my_vector_as_graph_traversal_tag
            : public vertex_list_graph_tag,
              public adjacency_graph_tag,
//              public random_access_traversal_tag,
              public bidirectional_graph_tag,
              public edge_list_graph_tag {
    };

    template<class WeightType>
    struct out_edge_iterator_policies;
//    struct vertex_iterator_policies;

    template<class WeightType>
    struct graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>> {
        typedef typename std::vector<std::pair<int, WeightType>>::value_type V;
        typedef int vertex_descriptor;
        typedef std::pair<int, std::pair<int, WeightType> *> edge_descriptor;

        // iterator typedefs...

        typedef directed_tag directed_category;
        typedef allow_parallel_edge_tag edge_parallel_category;
        typedef int vertices_size_type;
        typedef int edges_size_type;
        typedef my_vector_as_graph_traversal_tag traversal_category;

        class out_edge_iterator : public iterator_adaptor<
                out_edge_iterator,
                edge_descriptor,
                edge_descriptor,
                bidirectional_traversal_tag,
                edge_descriptor &, edge_descriptor *> {

        public:
            out_edge_iterator() : out_edge_iterator::iterator_adaptor_(std::make_pair(0, nullptr)) { }

            explicit out_edge_iterator(edge_descriptor p)
                    : out_edge_iterator::iterator_adaptor_(p) { }

            edge_descriptor operator*() {
                return this->base();
            }


        private:
            friend class boost::iterator_core_access;

            void increment() {
                ++this->base_reference().second;
            }

            void decrement() {
                ++this->base_reference().second;
            }
        };


        typedef integer_range<vertex_descriptor>::iterator vertex_iterator;
    };

    template<class WeightType>
    struct edge_property_type<std::vector<std::vector<std::pair<int, WeightType>>>> {
        typedef edge_weight_t type;
    };

    template<class WeightType>
    struct vertex_property_type<std::vector<std::vector<std::pair<int, WeightType>>>> {
        typedef void type;
    };

    template<class WeightType>
    struct graph_property_type<std::vector<std::vector<std::pair<int, WeightType>>>> {
        typedef void type;
    };

    template<class WeightType>
    typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::vertex_descriptor
    source(typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::edge_descriptor e,
           const std::vector<std::vector<std::pair<int, WeightType>>> &g) {
        return e.first;
    }

    template<class WeightType>
    typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::vertex_descriptor
    target(typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::edge_descriptor e,
           const std::vector<std::vector<std::pair<int, WeightType>>> &g) {
        return e.second->first;
    }

    template<class WeightType>
    inline std::pair<
            typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::out_edge_iterator,
            typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::out_edge_iterator>
    out_edges(typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::vertex_descriptor u,
              const std::vector<std::vector<std::pair<int, WeightType>>> &g) {
        typedef typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::out_edge_iterator Iter;

        return std::make_pair(Iter(std::make_pair(u, (std::pair<int, WeightType> *) (&g[u][0]))),
                              Iter(std::make_pair(u, (std::pair<int, WeightType> *) (&g[u][0]) +
                                                     g[u].size())));
    }

    template<class WeightType>
    std::pair<typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::vertex_iterator,
            typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::vertex_iterator>
    vertices(const std::vector<std::vector<std::pair<int, WeightType>>> &g) {
        typedef typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::vertex_iterator Iter;
        return std::make_pair(Iter(0), Iter(g.size()));
    }

    template<class WeightType>
    typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::vertices_size_type
    num_vertices(const std::vector<std::vector<std::pair<int, WeightType>>> &g) {
        return (int) g.size();
    }


    template<class WeightType>
    struct edge_weight_property_map
            : public put_get_helper<
                    boost::edge_weight_t,
                    edge_weight_property_map<WeightType>
            > {

        edge_weight_property_map<WeightType>() { }

        typedef WeightType value_type;
        typedef edge_weight_t reference;
        typedef typename graph_traits<std::vector<std::vector<std::pair<int, WeightType>>>>::edge_descriptor key_type;
        typedef boost::readable_property_map_tag category;  // RO because we can't add properties
        inline edge_weight_t operator[](key_type e) const {
            return boost::edge_weight_t(e.second->second);
        }

        inline edge_weight_t operator()(key_type e) const {
            return this->operator[](e);
        }
    };

    namespace detail {
        template<class WeightType>
        struct edge_property_map<std::vector<std::vector<std::pair<int, WeightType>>>, edge_weight_t> {
            typedef edge_weight_property_map<WeightType> type;
            typedef edge_weight_property_map<WeightType> const_type;
        };
    }

    template<class WeightType>
    identity_property_map
    get(vertex_index_t, const std::vector<std::vector<std::pair<int, WeightType>>> &) {
        return identity_property_map();
    }

    template<class WeightType>
    identity_property_map
    get(vertex_index_t, std::vector<std::vector<std::pair<int, WeightType>>> &) {
        return identity_property_map();
    }

    template<class WeightType>
    identity_property_map
    get(edge_index_t, const std::vector<std::vector<std::pair<int, WeightType>>> &) {
        return identity_property_map();
    }

    template<class WeightType>
    identity_property_map
    get(edge_index_t, std::vector<std::vector<std::pair<int, WeightType>>> &) {
        return identity_property_map();
    }


    template<class WeightType>
    typename detail::edge_property_map<std::vector<std::vector<std::pair<int, WeightType>>>, edge_weight_t>::const_type
    get(edge_weight_t, const std::vector<std::vector<std::pair<int, WeightType>>> &) {
        typedef typename detail::edge_property_map<std::vector<std::vector<std::pair<int, WeightType>>>, edge_weight_t>::const_type PA;
        return PA();
    }

    template<class WeightType>
    typename detail::edge_property_map<std::vector<std::vector<std::pair<int, WeightType>>>, edge_weight_t>::type
    get(edge_weight_t, std::vector<std::vector<std::pair<int, WeightType>>> &) {
        typedef typename detail::edge_property_map<std::vector<std::vector<std::pair<int, WeightType>>>, edge_weight_t>::type PA;
        return PA();
    }

}

#endif //VECTOR_AS_GRAPH_WITH_PROPERTY_VECTOR_AS_GRAPH_WITH_PROPERTY_H
