#ifndef __TSPLIB_PARSER_HPP__
#define __TSPLIB_PARSER_HPP__

#include <utility>
#include <vector>
#include <string>

#define TSPLIB_PARSER_NAMESPACE_BEGIN namespace tsplib_parser {
#define TSPLIB_PARSER_NAMESPACE_END } // namespace  tsp

#define UNNAMED_NAMESPACE_BEGIN namespace {
#define UNNAMED_NAMESPACE_END } // namespace  tsp

TSPLIB_PARSER_NAMESPACE_BEGIN

/// DESC
template <typename weight_type>
struct coord
{
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

    // generates the 2 matrix
    //std::vector<weight_type> generate_symmetric_input_matrix() const;

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

TSPLIB_PARSER_NAMESPACE_END

#endif // __TSPLIB_PARSER_HPP__
