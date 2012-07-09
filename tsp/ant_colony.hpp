#ifndef __TPS_ANT_COLONY_HPP__
#define __TPS_ANT_COLONY_HPP__

#include <algorithm>
#include <random>
#include <vector>
#include <list>
#include <functional>
#include <iostream> // temp

#include "adjacency_matrix.hpp"

namespace tsp {

//
// rng engine
// 

typedef std::mt19937 rng_engine_type;  
     
rng_engine_type& get_rng_engine()
{
    static rng_engine_type rng_engine;
    return rng_engine;
}

//
// pheromone_trails
//

template <typename weight_type>
class pheromone_trails
{
public:

    // ctor
    pheromone_trails(int dimension, weight_type init = weight_type())
        : dimension_(dimension), mat_(dimension*dimension, init)
    {}

    // getter
    const weight_type& operator()(int src, int dst) const
    {
        return mat_[src*dimension_ + dst];
    }

    // update with a tour
    void update(const tour& tour)
    {
        for (int i = 0; i < dimension_-1; i++)
        {
            int src = tour(i);
            int dst = tour(i+1);

            add(src, dst, weight_type(1));
        }
    }

    // evaporate a percentage from each edge
    void evaporate(float rate)
    {
        assert(rate >= 0.f && rate <= 1.f); 
        std::for_each(mat_.begin(), mat_.end(), [&](weight_type& p)
        { 
            p = weight_type(float(p) * rate); 
        });
    }

private:
    int dimension_;
    std::vector<weight_type> mat_;

    // add pheromone to an edge
    void add(int src, int dst, weight_type value)
    {
        mat_[src*dimension_ + dst] += value;
    }
};

//
// heuristics
// 

template <typename weight_type>
struct heuristic
{
    static const float default_distance_weight()  { return 0.80f; }
    static const float default_pheromone_weight() { return 0.15f; }
    static const float default_random_weight()    { return 0.05f; }

    // core values
    weight_type distance;     
    weight_type pheromone; 
    float random;

    // weight used by hueristic calculation
    float weight;

    // normalize and apply weights
    void update_weight(const weight_type& max_distance, const weight_type& max_pheromone)
    {
        // initialize
        weight = 0.f;
        
        // add distance component
        weight += default_distance_weight() * (1.f - float(distance) / float(max_distance));

        // add pheromone component
        if (max_pheromone && pheromone) 
            weight += default_pheromone_weight() * (float(pheromone) / float(max_pheromone));

        // add random component
        weight += default_random_weight() * random;

        assert( weight <= 1.0 );
    }
};

//
// ant
//

class ant
{
public:

    template <typename weight_type>
    tour visit(const adjacency_matrix<weight_type>& adjacency_matrix, const pheromone_trails<weight_type>& pheromone_trails)
    {
        // create tour
        int tour_size = adjacency_matrix.dimension();
        tour this_tour(tour_size);

        // randomize starting point
        std::uniform_int_distribution<int> int_dist(0, tour_size-1);
        std::uniform_real_distribution<float> float_dist(0, 1);

        int src = int_dist(get_rng_engine());
        this_tour.add_stop(src);

        // inialize valid location list
        std::list<int> tabu;
        for (int i = 0; i < tour_size; i++)
            if (i != src)
                tabu.push_back(i);

        // node hueristics
        typedef typename heuristic<weight_type> heuristic_type;
        typedef typename std::pair<int, heuristic_type> heuristic_pair;
        std::vector<heuristic_pair> heuristics;

        // begin tour...
        while (!tabu.empty())
        {
            // reset huerstic components
            heuristics.resize(tabu.size());

            // calculate the distance of each unvisited stop
            std::transform(tabu.begin(), tabu.end(), heuristics.begin(), [&](const int& dst)
            {
                heuristic_type h;
                int src = this_tour.current_location();

                // calcuate distance componet
                h.distance = adjacency_matrix(src, dst);

                // calculate pheromone componet
                h.pheromone = pheromone_trails(src, dst);

                // calculate pheromone component
                h.random = float_dist(get_rng_engine());

                return std::make_pair(dst, h);
            });

            // find maximum distance to normalize distance component
            weight_type max_distance = std::max_element(heuristics.begin(), heuristics.end(), [](const heuristic_pair& lhs, const heuristic_pair& rhs){ return lhs.second.distance < rhs.second.distance; } )->second.distance;

            // find maximum pheromone to normalize pheromone componet
            weight_type max_pheromone = std::max_element(heuristics.begin(), heuristics.end(), [](const heuristic_pair& lhs, const heuristic_pair& rhs){ return lhs.second.pheromone < rhs.second.pheromone; } )->second.pheromone;
    
            // calculate hueristics weights
            std::vector<float> hueristics_weights;
            std::for_each(heuristics.begin(), heuristics.end(), [&](heuristic_pair& hp)
            {
                hp.second.update_weight(max_distance, max_pheromone);
            });
                
            // find the best hueristic location
            int max_hueristic = std::max_element(heuristics.begin(), heuristics.end(), [](const heuristic_pair& lhs, const heuristic_pair& rhs){ return lhs.second.weight < rhs.second.weight; } )->first;

            // add to our tour
            this_tour.add_stop(max_hueristic);

            // remove stop from the valid list
            tabu.remove(max_hueristic);
        }

        return this_tour;
    } 

};

//
// colony
//

template <typename weight_type>
class ant_colony
{
public:
    ant_colony(const adjacency_matrix<weight_type>& adjacency_matrix, int ant_count)
        : adjacency_matrix_(adjacency_matrix), ant_count_(ant_count), pheromone_trails_(adjacency_matrix.dimension())
    {
    }

    tour run()
    {

        std::vector<tour> ant_tours;

        // generate ant trails
        for (int i = 0; i < ant_count_; i++)
            ant_tours.push_back( ant_.visit(adjacency_matrix_, pheromone_trails_) );

        // update the pheromone trails
        for (const tour& tour : ant_tours)
        {
            pheromone_trails_.update(tour);
        }

        // evaporate
        pheromone_trails_.evaporate(0.5f);

        // calculate output
        std::vector<weight_type> tour_distances;
        std::for_each(ant_tours.begin(), ant_tours.end(), [&](const tour& tour)
        {
            tour_distances.push_back(calculate_distance(adjacency_matrix_, tour));
        }); 

        // save best
        auto it = std::min_element(tour_distances.begin(), tour_distances.end());
        std::cout << "Min: " << *it << std::endl;

        // TMP
        return ant_tours[0];

    }

private:
    const adjacency_matrix<weight_type>& adjacency_matrix_;
    int ant_count_;
    pheromone_trails<weight_type> pheromone_trails_;

    ant ant_;
};

} // namespace tsp

#endif // __TPS_ANT_COLONY_HPP__