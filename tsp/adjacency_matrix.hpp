#ifndef __TPS_ADJACENCY_MATRIX_HPP__
#define __TPS_ADJACENCY_MATRIX_HPP__

#include <vector>

namespace tsp {

class adjacency_matrix
{

public:

    // ctor from std::vector<int>
    adjacency_matrix(const std::vector<int>& adj_mat, int nn_count = 32);

    // matrix contrains size*size elements
    int size() const;

    // number of nearest neighbors to store
    int nn_count() const;

    // full accessors    
    const int& operator()(int src, int dst) const;
    int& operator()(int src, int dst);

    // nearest neightbor accessors    
    const std::vector<int>& nn(int src) const;
    std::vector<int>& nn(int src);
    
private:

    // full adjacency matrix data
    std::vector<int> data_;

    // size of matrix
    int size_;

    // nearest neighbor count
    int nn_count_;

    // nearest neightbor data
    std::vector<std::vector<int>> nn_data_;

};

} // namespace tsp

#endif // __TPS_ADJACENCY_MATRIX_HPP__