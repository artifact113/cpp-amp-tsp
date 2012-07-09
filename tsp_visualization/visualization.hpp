#ifndef __TSP_VISUALIZATION_HPP__
#define __TSP_VISUALIZATION_HPP__

#include <memory>
#include <string>

#include "tsp/tour.hpp"
#include "tsplib_parser/tsplib_parser.hpp"

namespace viz {

template <typename weight_type>
class display
{
public:
    display(const tpslib::tsplib_data<weight_type>& input, int window_size = 640);

    static const int border_size = 16;

    // 
    void clear();
    void draw_nodes();
    void draw_tour(const tsp::tour& tour);

    // draw to screen
    void output_to_window(const std::string& name);

private:    
    // OpenCV header firewall
    struct impl;
    std::shared_ptr<impl> pimpl;
};

} // namespace viz

#endif // __TSP_VISUALIZATION_HPP__