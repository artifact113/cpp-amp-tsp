#ifndef __TSP_VISUALIZATION_HPP__
#define __TSP_VISUALIZATION_HPP__

#include <memory>

#include "tsp/tour.hpp"
#include "tsplib_parser/tsplib_parser.hpp"

namespace viz {

template <typename weight_type>
class display
{
public:
    display(const tpslib::tsplib_data<weight_type>& input, int window_size = 640);
    void overlay(const tsp::tour& tour);

private:    
    // OpenCV header firewall
    struct impl;
    std::shared_ptr<impl> pimpl;
};

} // namespace viz

#endif // __TSP_VISUALIZATION_HPP__