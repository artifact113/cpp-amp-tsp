#include <list>
#include <algorithm>
#include <utility>

// tmp
#include <iostream>

#include "ant.hpp"

#include "adjacency_matrix.hpp"
#include "pheromones.hpp"
#include "tour.hpp"
#include "colony.hpp"
#include "config.hpp"

namespace tsp {

ant::ant(const colony& colony)
    : colony_(colony)
{
   
}

void ant::do_tour()
{
    const adjacency_matrix& adj_mat = colony_.get_adjacency_matrix();

    // create a new tour
    tour this_tour(adj_mat);

    // randomize starting point
    std::uniform_int_distribution<int> int_dist(0, adj_mat.size()-1);
    int src = int_dist(rng_engine());
    this_tour.add_stop(src);

    // inialize valid location list; excluding current location
    std::list<int> tabu;
    for (int i = 0; i < adj_mat.size(); i++)
        if (i != src)
            tabu.push_back(i);

    // node hueristics
    typedef std::pair<int, float> heuristic_pair;
    std::vector<heuristic_pair> heuristic_pdf;

    // begin tour...
    while (!tabu.empty())
    {
        // reset huerstic components
        heuristic_pdf.resize(tabu.size());

        // get current location
        int src = this_tour.current_location();

        // calculate sum of all pheromone trails from this location
        // float pheromone_sum = weight_type();
        // std::for_each(tabu.begin(), tabu.end(), [&](const int& dst)
        // {
        //     if (src != dst)
        //         pheromone_sum += float(pheromone_trails(src, dst));
        // });
        // 
        // float distance_sum = weight_type();
        // std::for_each(tabu.begin(), tabu.end(), [&](const int& dst)
        // {
        //     if (src != dst)
        //         distance_sum += (1.f / float(adjacency_matrix(src, dst)));
        // });

        // create probability density function
        std::transform(tabu.begin(), tabu.end(), heuristic_pdf.begin(), [&](const int& dst)
        {
            float d = (1.f / float(adj_mat(src, dst)));  /// distance_sum;
            return std::make_pair(dst, d);
        });

        // get min
        int dst = std::max_element(heuristic_pdf.begin(), heuristic_pdf.end(), [](const heuristic_pair& lhs, const heuristic_pair& rhs)
        {
            return lhs.second < rhs.second;
        })->first;
            
        // add to our tour
        this_tour.add_stop(dst);

        // remove stop from the valid list
        tabu.remove(dst);
    }

    // complete the tour
    this_tour.finalize();

    // attempt to apply local optimizations
    this_tour.optimize();

    // better than our best?
    if ( !local_best_tour_ )
        local_best_tour_ .reset(new tour(this_tour)); 
    else if ( this_tour < get_local_best_tour() )
        local_best_tour_ .reset(new tour(this_tour)); 

} 

const tour& ant::get_local_best_tour() const
{
    return *local_best_tour_;
}

} // namespace tsp
