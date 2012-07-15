#ifndef __TSP_ANT_HPP__
#define __TSP_ANT_HPP__

#include <memory>

namespace tsp {

// forward
class colony;
class tour;

class ant
{

public:

    // ctor
    ant(const colony& colony);

    // perform a random tour using information from the colony
    void do_tour();

    // returns this ants best tour
    const tour& get_local_best_tour() const;

private:
    
    // reference to the colony this ant belongs to
    const colony& colony_;
    std::shared_ptr<tour> local_best_tour_;

};

} // namespace tsp

#endif __TSP_ANT_HPP__
