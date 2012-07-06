#ifndef __TSPLIB_PARSER_HPP__
#define __TSPLIB_PARSER_HPP__

#include <utility>
#include <vector>
#include <string>
#include <memory>

#include "tsp/forward.hpp"

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
