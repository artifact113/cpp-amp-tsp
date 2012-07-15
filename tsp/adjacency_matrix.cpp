#include <assert.h>
#include <algorithm>

#include "tsp/adjacency_matrix.hpp"

namespace {

struct distance_pair
{
    int id;     // node id
    int dist;   // distance to node

    distance_pair(int id, int dist)
        : id(id), dist(dist) 
    {}
};

}

namespace tsp {

adjacency_matrix::adjacency_matrix(const std::vector<int>& adj_mat, int nn_count)
    : data_(adj_mat)
{
    // get size
    size_ = int( std::sqrt(adj_mat.size() ));

    // neareast neightbor count
    nn_count_ = std::min(size_-1, nn_count);

    // make sure the input matrix was square
    assert( size_*size_ == adj_mat.size() );

    // allocate nearest neighbor data
    nn_data_.resize(size_);

    // allows for easier operator()
    adjacency_matrix& self = *this;

    // initailize nearest neighbor lists
    std::vector<distance_pair> neighbors;
    neighbors.reserve(size_ - 1);
    for (int src = 0; src < size(); src++)
    {
        // clear for next source
        neighbors.clear();

        // get the distance vector
        for (int dst = 0; dst < size(); dst++)
            if (src != dst)
                neighbors.push_back(distance_pair(dst, self(src,dst)));

        // sort by distance
        std::sort(neighbors.begin(), neighbors.end(), [](const distance_pair& lhs, const distance_pair& rhs)
        {
            return lhs.dist < rhs.dist;
        });
        
        // pre-allocate the list
        self.nn(src).reserve(nn_count_);
        
        // copy the first nn_count ids
        for (int nn = 0; nn < nn_count_; nn++)
            self.nn(src).push_back(neighbors[nn].id);
    }
}

int adjacency_matrix::size() const
{
    return size_;
}

int adjacency_matrix::nn_count() const
{
    return nn_count_;
}

int& adjacency_matrix::operator()(int src, int dst)
{
    return data_[src*size_+dst];
}

const int& adjacency_matrix::operator()(int src, int dst) const
{
    return data_[src*size_+dst];
}

std::vector<int>& adjacency_matrix::nn(int src)
{
    return nn_data_[src];
}

const std::vector<int>& adjacency_matrix::nn(int src) const
{
    return nn_data_[src];
}

} // namespace tsp
