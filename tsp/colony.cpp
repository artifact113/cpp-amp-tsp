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

pheromones& colony::get_pheromones()
{
    return pheromones_;
}

const pheromones& colony::get_pheromones() const
{
    return pheromones_;
}

int colony::iterate()
{   
    // simulate each ant in parallel
    std::for_each(ants().begin(), ants().end(), std::mem_fn(&ant::do_tour));

    // each tour generated contributes to the pheromate matrix
    for (const ant& ant : ants())
       pheromones_.update(ant.get_local_best_tour());

    // evaporate some pheromones
    pheromones_.evaporate(0.3f);
    
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

const tour& colony::worst_tour() const
{
    auto it = std::max_element(ants().begin(), ants().end(), [](const ant& lhs, const ant& rhs)
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
