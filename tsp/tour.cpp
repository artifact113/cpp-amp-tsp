#include <algorithm>
#include <assert.h>

#include "tour.hpp"
#include "config.hpp"

namespace tsp {

tour::tour(const adjacency_matrix& adjacency_matrix)
    : adjacency_matrix_(adjacency_matrix), length_(0)
{
    // +1 to complete circuit
    data_.reserve(adjacency_matrix_.size());
}

int tour::size() const 
{
    return adjacency_matrix_.size();
}

void tour::add_stop(int id)
{
    data_.push_back(id);
}

int tour::current_location() const
{
    return data_.back();
}

const int& tour::operator()(int i) const
{
    if (i < 0)
        return data_.back();
    else if ( i >= size() )
        return data_.front();
    else
        return data_.at(i);
}

bool tour::isFinal() const
{
    return data_.size() == size();
}

bool tour::isValid() const
{
    std::vector<int> check = data_;
    std::sort(check.begin(), check.end());
    return (check.end() == std::unique(check.begin(), check.end()));
}

int tour::length(bool force) const
{
    // make sure we are a complete route
    assert(isFinal());
    assert(isValid());

    const tour& self = *this;

    // if not cached or non-forced
    if (!length_ || force)
    {
        length_ = 0;
        int stops = size();

        // first n-1 stops
        for (int i = 0; i < stops; i++)
        {
            int src = self(i);
            int dst = self(i+1);
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
    const tour& self = *this;

    int a = self(n-1);
    int b = self(n);
    int c = self(n+1);

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
    for (const int& n0 : shuffle)
    {
        // get unoptimized destination and distance
        //const int dst = data_[idata[n0]+1];
        //int distance = adjacency_matrix_(n0, dst);

        // for each nearest neighbor
        for (const int& n1 : adjacency_matrix_.nn(n0))
        {
            // nearest neighbor data
            //int n1_distance = adjacency_matrix_(n0, n1);

            // if n1 is closer... 
            //if (distance > n1_distance)
            {   
                // measure before distance of effected nodes
                int input = symmetric_distance(idata[n0]) + symmetric_distance(idata[n1]);
                    
                // try swap
                std::swap( data_[idata[n0]], data_[idata[n1]] );

                // measure change
                int output = symmetric_distance(idata[n0]) + symmetric_distance(idata[n1]);
                int delta = input - output;

                if (delta > 0)
                {
                    break;
                }
                else
                {
                    // no improvement; swap back
                    std::swap( data_[idata[n0]], data_[idata[n1]] );
                }
            }
        }
    }
}

} // namespace tsp
