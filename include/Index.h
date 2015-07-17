#pragma once

#include <iostream>
#include <array>


// Exception raised when axes don't align but should
class DimensionException : public std::exception{
    virtual const char* what() const throw(){
        return "Input dimensions.";
    }
} AlignmentException;


template<size_t ndims, class IndexPrecision=long>
class Index
{
public:

	// Fill with default indices
	Index() { }

	// With
	Index(const std::array<IndexPrecision, ndims>& indices) :
		indices(indices) {}

	// Cross your fingers with this one
	Index(std::initializer_list<IndexPrecision> inds): indices(inds) {}
    
	Index(const IndexPrecision& val) {
		std::fill_n(indices.begin(), ndims, val);
	}

	// From another index
	//Index(const Index& other):
	//	indices(other.indices) {}
    
	// Move construct from an rvalue reference
	//Index(const Index& other) :
	//	indices(other.indices) { }

    // Data
    std::array<IndexPrecision,ndims> indices;
    
    // Accessors
    //const:
    const IndexPrecision& operator[](const size_t& dim) const {
        return indices[dim];
    }
    
    // non-const:
    IndexPrecision& operator[](const size_t& dim){
        return indices[dim];
    }
    
    // display
	void print() const {
		std::cout << "(";
		for (auto i = 0; i != ndims; ++i) {
			if (i > 0) {
				std::cout << "," << indices[i];
			}
			else {
				std::cout << indices[i];
			}
		}
		std::cout << ")";
	}
    
};
