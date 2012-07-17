#include <iostream>
#include <memory>

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

// parser
#include "tsplib_parser/tsplib_parser.hpp"

// simulation
#include "tsp/adjacency_matrix.hpp"
#include "tsp/colony.hpp"
#include "tsp/tour.hpp"

// viz
#include "tsp_visualization/visualization.hpp"

/*

template <typename coord_type>
class gui
{
public:
    gui(const tsplib::tsplib_data<coord_type>& data, tsp::ant_colony& colony)
        : output(data), colony(colony)
    {
        thread_ptr.reset(new std::thread(std::bind(&gui<coord_type>::run, this)));
    }

    ~gui()
    {
        thread_ptr->join();
    }

    void update(const tsp::tour& tour)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        f0 = std::bind(&gui<weight_type>::do_update, this, std::ref(tour));
        cond.notify_all();
    }

private:

    void run()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        while(true)
        {
            cond.wait(lock);
            f0();
        }
    }

    void do_update(const tsp::tour<weight_type>& tour)
    {
        output.clear();
        output.draw_trails(colony.trails());
        output.draw_tour(tour);   
        output.draw_nodes();
        output.output_to_window("Test");
    }

    std::shared_ptr<std::thread> thread_ptr;
    viz::display<weight_type> output;
    const tsp::ant_colony<weight_type>& colony;

    std::mutex mutex_;
    std::condition_variable cond;
    std::function<void(void)> f0; 
};

*/

int main()
{
    try
    {
        // read data
        tsplib::tsplib_data<int> data("st70.tsp");

        // get symmetrix adjacency matrix
        std::vector<int> vector_data = data.generate_adjacency_matrix();

        // create adjacency_matrix wrapper
        tsp::adjacency_matrix adj_mat(vector_data, 64);

        const int ant_count = 50;
        tsp::colony colony(adj_mat, ant_count);
        
        auto gui = colony.gui(data, 800);

        static const std::string window_name = "TSP";

        while( true )
        {
            auto i = colony.iterate();
            auto t = colony.best_tour();
            auto t0 = colony.worst_tour();
            auto l = t.length();
            std::cout << i << ": " << t.length() << " ... " << t0.length() << std::endl;

            gui.clear();
            //gui.draw_trails(colony.get_pheromones());
            gui.draw_nodes();
            
            gui.draw_tour(t);
            gui.output_to_window(window_name);
        }
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
