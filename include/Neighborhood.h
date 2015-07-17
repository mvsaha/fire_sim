#pragma once
#include <iostream>
#include <memory>
#include <assert.h>
#include <vector>
#include <stdexcept>

#include <array>

using namespace std;


// Neigbors define adjecency in the Grid
template<size_t n_neighbors>
class Neighborhood {
public:

	Neighborhood() : offsets(0){
	}

	Neighborhood(const std::vector<int>& dims,
		std::vector<std::vector<long>>& offsets) :
		n(n_neighbors), dims{dims}, offsets{ offsets } { }

	const size_t n; // Cardinality
    const std::vector<int> dims; // Dimensions that the offsets apply
    const std::vector<std::vector<long long>> offsets; // [cardinality][dim_idx]
};



const Neighborhood<0> LonelyNeighborhood;

//const Neighborhood<4> LonelyNeighborhood;

//const Neighborhood<4> MooreNeighborhood({ 0,1 }, { {-1,0},{0,1},{1,0},{0,-1} });

//const Neighborhood<8> VonNeumannNeighborhood({0,1},
//    {{  0, -1, 0, 1, -1, -1, 1,  1 },
//     { -1,  0, 1, 0, -1,  1, 1, -1 }});




