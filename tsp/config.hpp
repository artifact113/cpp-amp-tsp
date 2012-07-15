#ifndef __TSP_CONFIG_HPP__
#define __TSP_CONFIG_HPP__

#include <random>

namespace tsp {
    
// rng engine
typedef std::mt19937 rng_engine_type;   
inline rng_engine_type& rng_engine()
{
    static rng_engine_type rng;
    return rng;
}

// data type to use for the pheromone matrix and calculations
typedef float pheromone_type;

}

#endif // __TSP_CONFIG_HPP__
