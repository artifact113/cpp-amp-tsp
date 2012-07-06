#include <assert.h>
#include <algorithm>

/*

namespace tsp {

template <typename WeightType>
WeightType fitness(int dimension, const std::vector<WeightType>& adjacencyMatrix, const std::vector<int>& tour)
{
    WeightType tourDistance = 0;

    // perfom tour
    for (int i = 0; i < dimension-1; i++)
    {
        int src = tour[i];
        int dst = tour[i+1];
        tourDistance += adjacencyMatrix[dimension*dst + src];
    }

    // close loop stop
    int src = tour[dimension-1];
    int dst = tour[0];
    tourDistance += adjacencyMatrix[dimension*dst + src];

    return tourDistance;
}

template <typename WeightType>
TSPResutls<WeightType> SolveTSP(int dimension, const std::vector<WeightType>& adjacencyMatrix)
{
    // check size
    assert(dimension*dimension == adjacencyMatrix.size());

    // answer
    TSPResutls<WeightType> ret;

    //std::vector<int> tour(dimension);

    ret.tour.resize(dimension)
    
    // initialize
    for (int i = 0; i < dimension; i++)
       ret.tour[i] = i;

    // 
    std::random_shuffle(tour.begin(), tour.end());

    WeightType tourDistance = fitness(dimension, adjacencyMatrix, tour);

    return ret;
}

// template instantatiation
template TSPResutls<int> SolveTSP<int>(int, const std::vector<int>&);
template TSPResutls<float> SolveTSP<float>(int, const std::vector<float>&);

TSP_NAMESPACE_END

*/
