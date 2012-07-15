#include <algorithm>

// TEMP!
#include <iostream>

#include "tour.hpp"
#include "config.hpp"

namespace tsp {

tour::tour(const adjacency_matrix& adjacency_matrix)
    : adjacency_matrix_(adjacency_matrix), length_(0)
{
    // +1 to complete circuit
    data_.reserve(adjacency_matrix_.size() + 1);
}

int tour::size() const 
{
    return adjacency_matrix_.size();
}

void tour::add_stop(int id)
{
    data_.push_back(id);
}

void tour::finalize()
{
    assert(data_.size() == size());
    add_stop(data_.front());
}

int tour::current_location() const
{
    return data_.back();
}

const int& tour::operator()(int i) const
{
    return data_[i];
}

bool tour::isFinal() const
{
    return data_.size() == size() + 1;
}

int tour::length(bool force) const
{
    // make sure we are a complete route
    assert(isFinal());

    // if not cached or non-forced
    if (!length_ || force)
    {
        length_ = 0;
        int stops = size();

        // first n-1 stops
        for (int i = 0; i < stops; i++)
        {
            int src = data_[i];
            int dst = data_[i+1];
            length_ += adjacency_matrix_(src, dst);
        }
    }

    return length_;
}

bool tour::operator<(const tour& rhs)
{
    return length() < rhs.length();
}

int tour::symmetric_distance(int n) const
{
    int a = data_[n-1 < 0 ? size()-2 : n-1];
    int b = data_[n];
    int c = data_[n+1];

    return adjacency_matrix_(a,b) + adjacency_matrix_(b,c);
}

void tour::optimize()
{
    // tour must be completed to perform any local optimizations
    assert(isFinal());

    // inverse positions (e.g. city 7 is stop 12)
    std::vector<int> idata(size());
    for (const int& i : data_)  
        idata[data_[i]] = i;

    // get a random ordering of stops
    std::vector<int> shuffle = data_;
    std::random_shuffle(shuffle.begin(), shuffle.end());

    // for each starting city...
    for (const int& src : shuffle)
    {
        // get unoptimized destination and distance
        const int dst = data_[idata[src]+1];
        int distance = adjacency_matrix_(src, dst);

        // for each nearest neighbor
        for (const int& nn : adjacency_matrix_.nn(src))
        {
            // nearest neighbor data
            int nn_distance = adjacency_matrix_(src, nn);

            // if nn is closer... 
            if (distance > nn_distance)
            {   
                // measure before distance of effected nodes
                int input = symmetric_distance(idata[src]) + symmetric_distance(idata[nn]);
                    
                // try swap
                std::swap( data_[idata[src]], data_[idata[nn]] );

                // measure change
                int output = symmetric_distance(idata[src]) + symmetric_distance(idata[nn]);
                int delta = input - output;

                if (delta > 0)
                {
                    break;
                }
                else
                {
                    // no improvement; swap back
                    std::swap( data_[idata[src]], data_[idata[nn]] );
                }
            }
        }
    }
}

} // namespace tsp
