#include <algorithm>
#include <functional>

// tsp
#include "colony.hpp"
#include "ant.hpp"
#include "adjacency_matrix.hpp"
#include "tour.hpp"

// parse
#include "tsplib_parser/tsplib_parser.hpp"

// viz
#include "tsp_visualization/visualization.hpp"

namespace tsp {

colony::colony(const adjacency_matrix& adjacency_matrix, int ant_count)
    : adjacency_matrix_(adjacency_matrix), 
      ant_count_(ant_count),
      iter_(0),
      best_tour_(adjacency_matrix),
      pheromones_(adjacency_matrix)
{
    ants_ = std::make_shared<std::vector<ant>>(ant_count, *this);
}

std::vector<ant>& colony::ants()
{
    return *ants_;
}

const std::vector<ant>& colony::ants() const
{
    return *ants_;
}

const adjacency_matrix& colony::get_adjacency_matrix() const
{
    return adjacency_matrix_;
}

int colony::iterate()
{   
    std::for_each(ants().begin(), ants().end(), std::mem_fn(&ant::do_tour));

    /*
    std::vector<tour<weight_type>> ant_tours;

    // generate ant trails
    for (int i = 0; i < ant_count_; i++)
        ant_tours.push_back( ant_.visit(adjacency_matrix_, pheromone_trails_) );

    // update the pheromone trails
    for (const tour<weight_type>& tour : ant_tours)
    {
        pheromone_trails_.update(tour);
    }

    // evaporate
    pheromone_trails_.evaporate( float(p()) / 100.f );

    // save best
    auto it = std::min_element(ant_tours.begin(), ant_tours.end());
    std::cout << it->length() << std::endl;

    return *it;
    */

    return ++iter_;
}

// get the current best tour
const tour& colony::best_tour() const
{
    auto it = std::min_element(ants().begin(), ants().end(), [](const ant& lhs, const ant& rhs)
    {
        return lhs.get_local_best_tour().length() < rhs.get_local_best_tour().length();
    });

    return it->get_local_best_tour();
}

const viz::display<int>& colony::gui(const tsplib::tsplib_data<int>& input, int window_size)
{
    if (!gui_)
        gui_ = std::make_shared<viz::display<int>>(input, window_size);

    return *gui_;
}

} // namespace tsp
