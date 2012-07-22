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
    // global colony information
    const adjacency_matrix& adj_mat = colony_.get_adjacency_matrix();
    const pheromones& pheromones = colony_.get_pheromones();

    // create a new tour
    tour this_tour(adj_mat);
   
    std::uniform_real_distribution<float> float_dist(0.f, 1.f);

    // randomize starting point
    std::uniform_int_distribution<int> int_dist(0, adj_mat.size()-1);
    int src = int_dist(rng_engine());
    this_tour.add_stop(src);

    // inialize valid location list; excluding current location
    // TODO: this isn't really a tabu lists
    std::list<int> tabu;
    for (int i = 0; i < adj_mat.size(); i++)
        if (i != src)
            tabu.push_back(i);

    // calculate sum of all pheromone trails from this location
    float pheromone_sum = float();
    std::for_each(tabu.begin(), tabu.end(), [&](const int& dst)
    {
        if (src != dst)
            pheromone_sum += float(colony_.get_pheromones()(src, dst));
    });

    float distance_sum = float();
    std::for_each(tabu.begin(), tabu.end(), [&](const int& dst)
    {
        if (src != dst)
            distance_sum += (float(1) / float(adj_mat(src, dst)));
    });

    // node hueristics & probability density
    typedef std::pair<int, float> heuristic_pair;
    std::vector<heuristic_pair> nn_pdf(adj_mat.nn_count());

    // begin tour...
    while (!tabu.empty())
    {
        // get current location
        int src = this_tour.current_location();

        // target location
        int dst = -1;

        // create probability density function of visiting all of our neighbors
        float p_sum = 0.f;
        std::transform(adj_mat.nn(src).begin(), adj_mat.nn(src).end(), nn_pdf.begin(), [&](const int& nn)
        {
            if (std::find(tabu.begin(), tabu.end(), nn) != tabu.end())
                std::make_pair(nn, 0); 

            float d = (1.f / float(adj_mat(src, nn))) / distance_sum;
            float p = pheromones(src, nn) / pheromone_sum;

            // TODO: alpha / beta
            float pdf = std::pow(d, 0.8f) + std::pow(p, 0.2f);
            p_sum += p;

            return std::make_pair(nn, pdf);
        });

        if (p_sum)
        {
            float rand = float_dist(rng_engine()) * p_sum;

            int i = 0;
            float p_scan = 0.f;
            while( p_scan <= rand )
            {
                p_scan += nn_pdf[i].second;
                i++;
            }
            i--;

            int p_dst = nn_pdf[i].first;
            if ( std::find(tabu.begin(), tabu.end(), p_dst) != tabu.end() )
                dst = p_dst;
        }
        
        if (dst == -1)
        {
            // all nearest neighbors have been visisted; choose closest

            auto it = std::min_element(tabu.begin(), tabu.end(), [&](const int& rhs, const int& lhs)
            {
                return adj_mat(src,rhs) < adj_mat(src,lhs);
            });

            dst = *it;
        }
            
        // add to our tour
        assert(dst != -1);
        this_tour.add_stop(dst);

        // remove stop from the valid list
        tabu.remove(dst);
    }

    // make sure we finished with a valid tour
    assert(this_tour.isFinal());
    assert(this_tour.isValid());

    // apply local optimizations
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
