#include <iostream>
#include <fstream>

#include "tsplib_parser.hpp"

UNNAMED_NAMESPACE_BEGIN

template <typename weight_type> struct valid_edge_weight_type;
template <> struct valid_edge_weight_type<int>   { static const std::string value() { return "EUC_2D"; }};
template <> struct valid_edge_weight_type<float> { static const std::string value() { return "GEO";    }};

bool parse_string_tag(std::ifstream& ifs, const std::string& tag, const std::string& key, const std::string& line, std::string& value)
{
    if (tag.find(key) != std::string::npos)
    {
        // ignore any leading markings
        size_t found = line.find_first_not_of(": ");

        // set revelent value
        if (found != std::string::npos)
            value = line.substr(found);

        // found
        return true;
    }
    else
    {
        // not found
        return false;
    }
}

bool parse_int_tag(std::ifstream& ifs, const std::string& tag, const std::string& key, const std::string& line, int& value)
{
    if (tag.find(key) != std::string::npos)
    {
        // ignore any leading markings
        size_t found = line.find_first_not_of(": ");

        // set revelent value
        if (found != std::string::npos)
        {
            std::string temp = line.substr(found);
            value = atoi(temp.c_str());
        }

        // found
        return true;
    }
    else
    {
        // not found
        return false;
    }
}

template <typename coord_type>
bool parse_coordinates_tag(std::ifstream& ifs, const std::string& tag, const std::string& key, const std::string& line, int dimension, std::vector<coord_type>& data)
{
    if (tag.find(key) != std::string::npos)
    {
        // dummy data
        int id;

        for (int i = 0; i < dimension; i++)
        {
            coord_type c;
            ifs >> id >> c.x >> c.y;
            data[i] = c;
        }

        // found
        return true;
    }
    else
    {
        // not found
        return false;
    }
}

UNNAMED_NAMESPACE_END

TSPLIB_PARSER_NAMESPACE_BEGIN

template <typename weight_type>
tsplib_data<weight_type>::tsplib_data(const std::string& file_name)
{   
    std::ifstream ifs;

    // open the tsp file
    ifs.open(file_name.c_str());

    // start parsing
    while(true)
    {
        std::string tag;

        // get the identifier
        ifs >> tag;
        if (tag.empty())
            break;

        // get the rest of the line 
        std::string line;
        getline(ifs, line);

        if (parse_string_tag(ifs, tag, "NAME", line, name_)) 
            continue;

        if (parse_string_tag(ifs, tag, "COMMENT", line, comment_))
            continue;

        if (parse_string_tag(ifs, tag, "EDGE_WEIGHT_TYPE", line, edgeweight_type_))
        {
            if (edgeweight_type_ != valid_edge_weight_type<weight_type>::value())
                throw std::runtime_error("Weight type mismatch");
            continue;
        }

        if (parse_string_tag(ifs, tag, "DISPLAY_DATA_TYPE", line, displayDataType_))
            continue;

        if (parse_string_tag(ifs, tag, "TYPE", line, type_))
            continue;

        if (parse_int_tag(ifs, tag, "DIMENSION", line, dimension_))
        {
            coordinates_.resize(dimension_);
            continue;
        }

        if (parse_coordinates_tag<coord_type>(ifs, tag, "NODE_COORD_SECTION", line, dimension_, coordinates_))
            continue;
    }
    
}

/*
template <typename weight_type>
std::vector<weight_type> tsplib_data<weight_type>::generate_symmetric_input_matrix() const
{
    std::vector<weight_type> distanceMatrix(dimension * dimension, weight_type());

    for (const coord& src : coordinates)
    {
        for (const coord& dst : coordinates)
        {
            // skip self and symmetric data
            if (src.id >= dst.id)
                continue;

            // caclulate symmetric distance
            weight_type d = src.distance(dst);
            distanceMatrix[(src.id-1) * dimension + (dst.id-1)] = d;
            distanceMatrix[(dst.id-1) * dimension + (src.id-1)] = d;
        }
    }

    return distanceMatrix;
}
*/

// Template Instantiations
template class tsplib_data<int>;
template class tsplib_data<float>;

TSPLIB_PARSER_NAMESPACE_END