#include "visualization.hpp"

#include "opencv2/opencv.hpp"

#include "tsp/tour.hpp"
#include "tsp/colony.hpp"
#include "tsplib_parser/tsplib_parser.hpp"

namespace viz
{
 
//
// Impl
//

template <typename weight_type>
struct display<weight_type>::impl
{
    // ctor
    impl(const tsplib::tsplib_data<weight_type>& input, int window_size)
        : input(input), window_size(window_size)
    {}

    // copy of input data
    const tsplib::tsplib_data<weight_type>& input;

    // background
    cv::Mat image;

    // various sizes
    int window_size;
    float x_scale;
    float y_scale;

    // border around the display window
    static const int border_size() { return 16; }

    // members
    cv::Point to_point(const tsplib::coord<weight_type>& c)
    {
        return cv::Point(int(c.x*x_scale)+border_size(), int(c.y*y_scale)+border_size());
    }
};

template <typename weight_type>
display<weight_type>::display(const tsplib::tsplib_data<weight_type>& input, int window_size)
{
    // create pimpl
    pimpl = std::unique_ptr<impl>(new impl(input, window_size));

    const int border_size = 16;

    int input_width = input.max_x();
    int input_height = input.max_y();

    pimpl->x_scale = float(window_size - pimpl->border_size()*2) / float(input_width);
    pimpl->y_scale = float(window_size - pimpl->border_size()*2) / float(input_height);
    
    // initialize image
    pimpl->image = cv::Mat::zeros(window_size, window_size, CV_8UC3);
}

template <typename weight_type>
void display<weight_type>::clear()
{
    // white background
    cv::rectangle(pimpl->image, cv::Point(0,0), cv::Point(pimpl->window_size, pimpl->window_size), cv::Scalar(255, 255, 255), -1);
}

template <typename weight_type>
void display<weight_type>::draw_nodes()
{    
    // draw nodes
    for (int i = 0; i < pimpl->input.points(); i++)
    {
        tsplib::coord<weight_type> c = pimpl->input(i);
        cv::circle(pimpl->image, pimpl->to_point(c), 5, cv::Scalar(0, 0, 255), -1);
    }
}

template <typename weight_type>
void display<weight_type>::draw_tour(const tsp::tour& tour)
{
    for (int i = 0; i < tour.size(); i++)
    {
        int src = tour(i);
        int dst = (i == tour.size()-1 ? tour(0) : tour(i+1));
        
        tsplib::coord<weight_type> c1 = pimpl->input(src);
        tsplib::coord<weight_type> c2 = pimpl->input(dst);
        cv::line(pimpl->image, pimpl->to_point(c1), pimpl->to_point(c2), cv::Scalar(0, 0, 128), 2);
    }
}

template <typename weight_type>
void display<weight_type>::draw_trails(const tsp::pheromones& trails)
{
    // get max to normalize color values
    float max = float(trails.max_value());

    // skip drawing super low levels
    const float min_level_to_draw = 1.f / trails.size();

    for (int src = 0; src < trails.size(); src++)
    {
        for (int dst = 0; dst < trails.size(); dst++)
        {
            // normalize and draw
            float level = float(trails(src,dst)) / max;
            if (level > min_level_to_draw)
            {
                int color = 255 - int(128.f * level);
                int thickness = int(5.f * level);
                tsplib::coord<weight_type> c1 = pimpl->input(src);
                tsplib::coord<weight_type> c2 = pimpl->input(dst);
                cv::line(pimpl->image, pimpl->to_point(c1), pimpl->to_point(c2), cv::Scalar(color, color, color), thickness);
            }
        }
    }
}

template <typename weight_type>
void display<weight_type>::output_to_window(const std::string& name)
{
    cv::imshow(name.c_str(), pimpl->image);
    cv::waitKey(0);
}

template class display<int>;
template class display<float>;

}