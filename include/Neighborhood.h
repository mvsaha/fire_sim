#pragma once
#include <iostream>
#include <memory>
#include <assert.h>
#include <vector>
#include <stdexcept>
#include <array>

// Define which Elements are adjacent in a Grid
template<size_t n_neighbors>
class Neighborhood {
public:
	Neighborhood(const std::vector<int> dims,
		const std::vector<std::vector<long>> offsets) :
    n(n_neighbors), dims(dims), offsets(offsets) { 
		assert(offsets.size()==n);
	}

    const size_t n;
    const std::vector<int> dims;
    const std::vector<std::vector<long>> offsets;
};


// Some pre-defined neighborhoods to get you started:
// Lonely neighborhood is defined as no pixels
// Think about using a Grid instead of a GridConnected
const Neighborhood<0> LonelyNeighborhood({}, {});


// VonNeumann Neighborhood around a pixel p:
//    0 
//  3 p 1 
//    2
const Neighborhood<4> VonNeumannNeighborhood({0,1}, { {-1,0},{0,1},{1,0},{0,-1} });


// Moore Neighborhood around a pixel p:
//  7 0 4 
//  3 p 1 
//  6 2 5
const Neighborhood<8> MooreNeighborhood({0,1}, { {-1,0},{0,1},{1,0},{0,-1},{-1,-1},{-1,1},{1,1},{1,-1} });
