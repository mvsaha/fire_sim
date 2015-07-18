#pragma once

#include <iostream>
#include <array>


// Exception raised when axes don't align but should
class DimensionException : public std::exception{
    virtual const char* what() const throw(){
        return "Input dimensions.";
    }
} AlignmentException;


template<size_t ndims, class IndexPrecision>
class Index
{
public:
    
	//========================================================
	//                      Constructors
	//========================================================

	// Fill with default indices
	Index() { }

	// Fill with an array
	Index(const std::array<IndexPrecision, ndims>& indices) :
		indices(indices) {}
    
    // With a single value for all fields
	Index(const IndexPrecision& val) {
		std::fill_n(indices.begin(), ndims, val);
	}

	template<class ...I>
	Index(IndexPrecision ind1, I&&... inds) noexcept :
	indices{ ind1 ,std::forward<I>(inds)... } {
		static_assert(sizeof...(I) == (ndims - 1), "Not the right number of indices");
	}
    
	//========================================================
	//                      Properties
	//========================================================
    std::array<IndexPrecision,ndims> indices;
    
    
	//========================================================
	//                      Accessors
	//========================================================
    //const
    const IndexPrecision& operator[](const size_t& dim) const {
        return indices[dim];
    }
    
    // non-const
    IndexPrecision& operator[](const size_t& dim){
        return indices[dim];
    }
    
    
	//========================================================
	//                      Display
	//========================================================
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
