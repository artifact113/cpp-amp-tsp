#include <algorithm>

#include "pheromones.hpp"

#include "adjacency_matrix.hpp"
#include "tour.hpp"

namespace {

    template <typename T>
    inline void clamp(T& val, const T& min, const T& max)
    {
        val = std::min(std::max(val, min), max);
    }

}

namespace tsp {

pheromones::pheromones(const adjacency_matrix& adjacency_matrix, pheromone_type init_value)
    : size_(adjacency_matrix.size()), mat_(size_*size_, init_value)
{
}

int pheromones::size() const 
{
    return size_;
}

const pheromone_type& pheromones::operator()(int src, int dst) const
{        
    return mat_[src*size_ + dst];
}

void pheromones::update(const tour& tour, pheromone_type weight)
{
    assert(tour.isFinal());
    assert(tour.isValid());

    pheromone_type tau = weight / tour.length();

    for (int i = 0; i < tour.size(); i++)
    {
        int src = tour(i);
        int dst = tour(i+1);

        pheromone_type value = mat_[src*size_ + dst] += tau;

        mat_[src*size_ + dst] = value;
        mat_[dst*size_ + src] = value;
    }
}

// evaporate a percentage from each edge
void pheromones::evaporate(float rate)
{
    assert(rate >= 0.f && rate <= 1.f); 
    
    for (pheromone_type& p : mat_)
    { 
        p = pheromone_type((1.f-rate) * p); 
    }
}

// use for normalization
pheromone_type pheromones::max_value() const
{
    return *std::max_element(mat_.begin(), mat_.end());
}

} // namespace tsp
