#include <iostream>

#include "tsplib_parser/tsplib_parser.hpp"
#include "tsp/adjacency_matrix.hpp"
#include "tsp/solve.hpp"

#include "tsp_visualization/visualization.hpp"

int main()
{
    try
    {
        // read data
        tpslib::tsplib_data<int> data("st70.tsp");
        viz::display<int> output(data);

        // get symmetrix adjacency matrix
        auto adjaceny_matrix = data.generate_adjacency_matrix();


        auto results = tsp::solve(*adjaceny_matrix.get());
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
    }
    catch(const std::exception& e)
    {
         std::cerr << "Error: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown Error" << std::endl;
    }

	return 0;
}
