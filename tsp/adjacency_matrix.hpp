#ifndef __TPS_ADJACENCY_MATRIX_HPP__
#define __TPS_ADJACENCY_MATRIX_HPP__

#include <vector>

#include "forward.hpp"

namespace tsp {

template <typename weight_type>
class adjacency_matrix
{
public:

    // ctor
    adjacency_matrix(int dimension);

    // accessors
    const weight_type& operator()(int src, int dst) const;
    weight_type& operator()(int src, int dst);
    
private:

    int dimension_;
    std::vector<weight_type> data_;
};

} // namespace tsp

#endif // __TPS_ADJACENCY_MATRIX_HPP__