#ifndef __TSP_VISUALIZATION_HPP__
#define __TSP_VISUALIZATION_HPP__

#include <memory>
#include <string>

// fowards
namespace tsp { class tour; }
namespace tsp { class pheromones; }
namespace tsplib { template <typename coord_type> class tsplib_data; }

namespace viz {

template <typename coord_type>
class display
{
public:

    // ctor
    display(const tsplib::tsplib_data<coord_type>& input, int window_size = 860);

    // construction functions
    void clear();
    void draw_nodes();
    void draw_tour(const tsp::tour& tour);
    void draw_trails(const tsp::pheromones& pheromones);

    // draw to screen
    void output_to_window(const std::string& name);

private:    
    // OpenCV header firewall
    struct impl;
    std::shared_ptr<impl> pimpl;
};

} // namespace viz

#endif // __TSP_VISUALIZATION_HPP__