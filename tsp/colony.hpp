#ifndef __TPS_ANT_COLONY_HPP__
#define __TPS_ANT_COLONY_HPP__

#include <vector>
#include <memory>

#include "pheromones.hpp"
#include "tour.hpp"

// forwards

namespace tsp {
    class adjacency_matrix;
    class ant;
}

namespace viz {
    template <typename T>
    class display;
}

namespace tsplib {
    template <typename T>
    class tsplib_data;
}

namespace tsp {

//
// Colony
//
// Holds the ants and their 
//

class colony
{

public:

    // ctor
    colony(const adjacency_matrix& adjacency_matrix, int ant_count = 50);
    
    // perform a colony iteration; returns iteration count
    int iterate();

    // get the current best tour
    const tour& best_tour() const;

    // read only adjacency matrix access
    const adjacency_matrix& get_adjacency_matrix() const;

    // read only pheromones access
    const pheromones& get_pheromones() const;

    // gui initialization and access
    const viz::display<int>& gui(const tsplib::tsplib_data<int>& input, int window_size);
        
private:

    // data access
    std::vector<ant>& ants();
    const std::vector<ant>& ants() const;

    // data
    const adjacency_matrix& adjacency_matrix_;
    int ant_count_;
    int iter_;
    tour best_tour_;
    pheromones pheromones_;
    std::shared_ptr<std::vector<ant>> ants_;

    // gui
    std::shared_ptr<viz::display<int>> gui_;

};

} // namespace tsp

#endif // __TPS_ANT_COLONY_HPP__