#pragma once
#include <iostream>
#include <memory>
#include <assert.h>
#include <vector>
#include <stdexcept>

#include <array>

using namespace std;


// Neigbors define adjecency in the Grid
//template<size_t n_neighbors>
class Neighborhood {
public:

	Neighborhood(const std::vector<int>& dims, const std::vector<std::vector<long>>& offsets) :
    n(offsets.size()), dims(dims), offsets{offsets} { }
    
    size_t n;
    std::vector<int> dims;
    std::vector<std::vector<long>> offsets;
};

const Neighborhood LonelyNeighborhood({},{{}}); // No neighbors

const Neighborhood MooreNeighborhood({0,1},{{-1,0},{0,1},{1,0},{0,-1}});

const Neighborhood VonNeumannNeighborhood({0,1},{{-1,0},{0,1},{1,0},{0,-1},{-1,-1},{1,-1},{1,1},{-1,1}});




