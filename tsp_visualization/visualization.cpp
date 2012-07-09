#include "visualization.hpp"

#include "opencv2/opencv.hpp"

namespace viz
{

template <typename weight_type>
struct display<weight_type>::impl
{
    // ctor
    impl(const tpslib::tsplib_data<weight_type>& input, int window_size)
        : input(input), window_size(window_size)
    {}

    //
    const tpslib::tsplib_data<weight_type>& input;

    // background
    cv::Mat image;

    // 
    int window_size;
    float x_scale;
    float y_scale;
};

template <typename weight_type>
display<weight_type>::display(const tpslib::tsplib_data<weight_type>& input, int window_size)
{
    // create pimpl
    pimpl = std::unique_ptr<impl>(new impl(input, window_size));

    const int border_size = 16;

    int input_width = input.max_x();
    int input_height = input.max_y();

    pimpl->x_scale = float(window_size - border_size*2) / float(input_width);
    pimpl->y_scale = float(window_size - border_size*2) / float(input_height);
    
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
        // void circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
        tpslib::coord<weight_type> c = pimpl->input(i);
        cv::circle(pimpl->image, cv::Point(int(c.x*pimpl->x_scale)+border_size, int(c.y*pimpl->y_scale)+border_size), 2, cv::Scalar(0, 0, 255), -1);
    }
}

template <typename weight_type>
void display<weight_type>::draw_tour(const tsp::tour& tour)
{
    for (int i = 0; i < tour.dimension(); i++)
    {
        int src = tour(i);
        int dst = (i == tour.dimension()-1 ? tour(0) : tour(i+1));
        
        tpslib::coord<weight_type> c1 = pimpl->input(src);
        tpslib::coord<weight_type> c2 = pimpl->input(dst);

        cv::Point pt1(int(c1.x*pimpl->x_scale)+border_size, int(c1.y*pimpl->y_scale)+border_size);
        cv::Point pt2(int(c2.x*pimpl->x_scale)+border_size, int(c2.y*pimpl->y_scale)+border_size);

        cv::line(pimpl->image, pt1, pt2, cv::Scalar(0,0,128), 1);
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