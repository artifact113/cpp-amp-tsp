#ifndef __TSP_PHEROMONE_HPP__
#define __TSP_PHEROMONE_HPP__

#include <vector>

#include "config.hpp"

namespace tsp {

// forwards
class tour;
class adjacency_matrix;

class pheromones
{

public:

    // ctor
    pheromones(const adjacency_matrix& adjacency_matrix, float init = pheromone_type(1));

    // dimension
    int size() const;

    // read only access
    const float& operator()(int src, int dst) const;
    
    // update with information from  tour
    void update(const tour& tour, float weight = pheromone_type(1));

    // evaporate a percentage from each edge
    void evaporate(float rate);

    // use for normalization
    float max_value() const;

private:

    int size_;
    std::vector<pheromone_type> mat_;

    // min-max optimization
    // pheromone_type min_;
    // pheromone_type max_;

};

} // namespace tps

#endif // __TSP_PHEROMONE_HPP__