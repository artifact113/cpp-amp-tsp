#include "tsp/adjacency_matrix.hpp"

namespace tsp {

template <typename weight_type>
adjacency_matrix<weight_type>::adjacency_matrix(int dimension)
    : dimension_(dimension), data_(dimension*dimension, weight_type())
{
}

template <typename weight_type>
adjacency_matrix<weight_type>::adjacency_matrix(const std::vector<weight_type>& mat)
    : dimension_(mat.size()), data_(mat)
{
}

template <typename weight_type>
int adjacency_matrix<weight_type>::dimension() const
{
    return dimension_;
}

template <typename weight_type>
weight_type& adjacency_matrix<weight_type>::operator()(int src, int dst)
{
    return data_[src*dimension_+dst];
}

template <typename weight_type>
const weight_type& adjacency_matrix<weight_type>::operator()(int src, int dst) const
{
    return data_[src*dimension_+dst];
}

template class adjacency_matrix<int>;
template class adjacency_matrix<float>;

} // namespace tsp
