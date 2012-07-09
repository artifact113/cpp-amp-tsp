#include "visualization.hpp"

#include "opencv2/opencv.hpp"

namespace viz
{

template <typename weight_type>
struct display<weight_type>::impl
{
    // background
    cv::Mat background;

    // background + tour overlay
    cv::Mat display;
};

template <typename weight_type>
display<weight_type>::display(const tpslib::tsplib_data<weight_type>& input, int window_size)
{
    // create pimpl
    pimpl = std::unique_ptr<impl>(new impl);

    const int border_size = 16;

    int input_width = input.max_x();
    int input_height = input.max_y();

    float x_scale = float(window_size - border_size*2) / float(input_width);
    float y_scale = float(window_size - border_size*2) / float(input_height);
    
    // initialize window
    cv::Mat image = cv::Mat::zeros(window_size, window_size, CV_8UC3);

    // white background
    cv::rectangle(image, cv::Point(0,0), cv::Point(window_size, window_size), cv::Scalar(255, 255, 255), -1);

    // draw nodes
    for(int i = 0; i < input.points(); i++)
    {
        // void circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
        tpslib::coord<weight_type> c = input(i);
        cv::circle(image, cv::Point(int(c.x*x_scale)+border_size, int(c.y*y_scale)+border_size), 2, cv::Scalar(0, 0, 255), -1);
    }

    // draw windows
    cv::imshow("test", image);
    cv::waitKey(0);
}

template <typename weight_type>
void display<weight_type>::overlay(const tsp::tour& tour)
{
}

template class display<int>;
template class display<float>;

}