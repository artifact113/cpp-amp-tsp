#ifndef __TPS_RESULTS_HPP__
#define __TPS_RESULTS_HPP__

#include <vector>

namespace tsp {

class tour
{
public:
    tour(int dimension)
    {
        data_.reserve(dimension);
    }

    int dimension() const 
    {
        return data_.size();
    }

    void add_stop(int id)
    {
        data_.push_back(id);
    }

    int current_location() const
    {
        return data_.back();
    }

    const int& operator()(int i) const
    {
        return data_[i];
    }

private:
    std::vector<int> data_;    
};

} // namespace tsp

#endif // __TPS_RESULTS_HPP__