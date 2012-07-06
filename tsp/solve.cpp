#include <assert.h>
#include <memory>

#include "tsp/solve.hpp"
#include "tsp/adjacency_matrix.hpp"
#include "tsp/tour.hpp"
#include "tsp/ant_colony.hpp"

namespace tsp {

template <typename weight_type>
weight_type calculate_distance(const adjacency_matrix<weight_type>& adjacency_matrix, const tour& tour)
{
    assert(adjacency_matrix.dimension() == tour.dimension());

    // initialize
    weight_type distance = weight_type();
    int stops = adjacency_matrix.dimension();

    // first n-1 stops
    for (int i = 0; i < stops-1; i++)
    {
        int src = tour[i];
        int dst = tour[i+1];
        distance += adjacency_matrix(src, dst);
    }

    // connect the circuit
    distance += adjacency_matrix(stops, 0);
}

template <typename weight_type>
std::unique_ptr<tour> solve(const adjacency_matrix<weight_type>& adjacency_matrix)
{
    int stops = adjacency_matrix.dimension();
    auto ret = std::unique_ptr<tour>(new tour(stops));

    const int ants = 10;
    ant_colony<weight_type> ac(adjacency_matrix, ants);

    return ret;
}

// template instantiations
template std::unique_ptr<tour> solve<int>(const adjacency_matrix<int>&);
template std::unique_ptr<tour> solve<float>(const adjacency_matrix<float>&);

}

