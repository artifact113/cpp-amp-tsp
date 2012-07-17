#ifndef __TPS_RESULTS_HPP__
#define __TPS_RESULTS_HPP__

#include <vector>
#include <assert.h>

#include "tsp/adjacency_matrix.hpp"

namespace tsp {

class tour
{

public:

    // ctor
    tour(const adjacency_matrix& adjacency_matrix);

    // number of stops (+1 for circuit)
    int size() const;

    // insert a stop to the tour
    void add_stop(int id);

    // checks to see if a full tour has been executed
    bool isFinal() const;

    // make sure all elements in the tour are unique
    bool isValid() const;

    // current stop of the tour
    int current_location() const;

    // real only access
    const int& operator()(int i) const;

    // total length of tour
    int length(bool force = false) const;

    // sort by length
    bool operator<(const tour& rhs);

    // return distance of {n-1, n, n+1} where n is an index
    int symmetric_distance(int src) const;

    // perform local optimzations (2-opt)
    void optimize();

private:

    const adjacency_matrix& adjacency_matrix_;
    std::vector<int> data_;    
    mutable int length_; 

};

} // namespace tsp

#endif // __TPS_RESULTS_HPP__