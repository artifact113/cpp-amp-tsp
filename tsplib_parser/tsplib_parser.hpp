#ifndef __TSPLIB_PARSER_HPP__
#define __TSPLIB_PARSER_HPP__

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

#include "tsp/adjacency_matrix.hpp"

namespace tpslib {

/// DESC
template <typename weight_type>
struct coord
{
    int id;
    weight_type x;
    weight_type y;
};

/// DESC
template <typename weight_type>
weight_type distance(const coord<weight_type>& rhs, const coord<weight_type>& lhs)
{
    return weight_type( sqrt( (rhs.x-lhs.x)*(rhs.x-lhs.x) + (rhs.y-lhs.y)*(rhs.y-lhs.y) ) );
}

/// DESC
template <typename weight_type>
class tsplib_data
{
    typedef typename coord<weight_type> coord_type;

public:

    // ctor
    tsplib_data(const std::string& file_name);

    // generates the an adjacey matrix from the data
    std::unique_ptr<tsp::adjacency_matrix<weight_type>> generate_adjacency_matrix() const;

    // get problem dimension 
    int dimension() const
    {
        return dimension_;
    }

    // coord access
    int points() const 
    { 
        return coordinates_.size(); 
    }

    const coord_type& operator()(int i) const 
    { 
        return coordinates_[i];
    }

    int max_x() const 
    { 
        auto it = std::max_element(coordinates_.begin(), coordinates_.end(), [](const coord_type& rhs, const coord_type& lhs){ return rhs.x < lhs.x; });
        return int(std::ceil(it->x));
    }

    int max_y() const 
    { 
        auto it = std::max_element(coordinates_.begin(), coordinates_.end(), [](const coord_type& rhs, const coord_type& lhs){ return rhs.y < lhs.y; });
        return int(std::ceil(it->y));
    }

private:

    std::string name_;
    std::string type_;
    std::string comment_;
    int dimension_;
    std::string edgeweight_type_;
    std::string displayDataType_;
    std::vector<coord_type> coordinates_;
};

} // namespace tpslib 

#endif // __TSPLIB_PARSER_HPP__
