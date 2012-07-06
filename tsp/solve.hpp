#ifndef __TPS_SOLVE_TSP_HPP__
#define __TPS_SOLVE_TSP_HPP__

#include "adjacency_matrix.hpp"
#include "tour.hpp"

namespace tsp {

template <typename weight_type>
weight_type calculate_distance(const adjacency_matrix<weight_type>& adjacency_matrix, const tour& tour);

template <typename weight_type>
std::unique_ptr<tour> solve(const adjacency_matrix<weight_type>& adjacency_matrix);

} // namespace tsp

#endif // __TPS_FORWARD_HPP__