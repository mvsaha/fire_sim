#pragma once

#include "Element.h"


// Exception raised when bounds are invalid
class OutOfBoundsException : public std::exception{
	virtual const char* what() const throw(){
		return "Index exceeded Grid Size.";
	}
} OutOfBoundsException;


enum class BOUNDARY {
	NONPERIODIC,
	PERIODIC,
};

// Represents a n-dimensional lattice
template<class DataType,size_t ndims,class IndexPrecision=long>
class Grid
{
public:
	friend Element<DataType,ndims,IndexPrecision>;
	Neighborhood neighborhood;
	std::vector<Element<DataType,ndims,IndexPrecision>> elements;
	static const Element<DataType, ndims,IndexPrecision> NONE;
	
	const std::array<IndexPrecision, ndims>& dimensions;
	const std::array<BOUNDARY, ndims>& boundaries;

	// How many elements a single increment represents
	// Column major ordering means that this will be larger for 
	// smaller dimensions
	std::array<IndexPrecision,ndims> dim_stride;

    Grid(const std::array<IndexPrecision,ndims>& dimensions,
		 const std::array<BOUNDARY,ndims>& boundaries,
		 const Neighborhood& N,DataType fill=0):
		dimensions(dimensions), boundaries(boundaries), neighborhood(N)
    {
		assert(ndims == dimensions.size()); // Inputs must match

		dim_stride[ndims - 1] = 1;
		for (auto dim = ndims - 2; dim != -1; --dim) {
			dim_stride[dim] = dim_stride[dim + 1] * dimensions[dim];
		}
		elements.reserve(dim_stride[0]*dimensions[0]);

		// Now populate elements
		Index<ndims, IndexPrecision> idx({ 0,0,0 });
		bool done = false;
		
		while (!done) {
			elements.push_back(Element<DataType, ndims, IndexPrecision>(idx,fill,neighborhood.n,true));

			for (auto dim = ndims - 1; dim != -1; --dim) {
				idx[dim] += 1;
				if (idx[dim] > dimensions[dim] ) {
					if (dim == 0) {
						done = true;
						break;
					} // if we overflow in dimension 0 then were done
					idx[dim] = 0; // Else roll over to zero
					idx[dim - 1] += 1;
					break;
				}
			}
		}
		//
		
		link_elements();
    }

	void link_elements() {
		//Loop over elements
		for (auto e : elements) {
			
			//Loop over neighbors
			for (auto n = 0; n != neighborhood.n; ++n) {
				// Calculate index after ofset, if its valid (with periodicity)
				// then assign it
			}
		}
	}

	// Assign neighbors
	/*
	void link_elements() {
		for (long long iy = 0; iy != Y; ++iy) {
			for (long long ix = 0; ix != X; ++ix) {

				auto element = this->operator()(iy, ix);
				element.print();
				cout << " --- ";
				auto n_neighbors = static_cast<int>(C);
				assert(element.neighbors.size() == n_neighbors);
				for (auto N = 0; N != n_neighbors; ++N) {
					
					// Get the element or return NONE
					auto neighbor = this->bound(iy+y_neighbors[N],
								  			    ix+x_neighbors[N]);
					neighbor.print();
					element.neighbors[N] = &neighbor;
					element.neighbor(N).print();
					cout << "  ,  ";

				}

				cout << endl;
			}
		}
	}*/

	
	inline const IndexPrecision bound_axis(IndexPrecision val, int dim) const {
		// Returns -1 if val is not in range [0,size_of_dim) and the dimension
		// is not periodic.
		// Otherwise it just returns bounded val.

		if (val >= size[dim]) {
			if (boundaries[dim]==BOUNDARY::PERIODIC) {
				return val%B;}
			else {
				return -1;}
		}
		else if (val < 0) {
			if (boundaries[dim]==BOUNDARY::PERIODIC) {
				val *= -1; // val must be positive now
				return B - (val%B);
			}
			else {
				return -1;}
		}
		return val;
	}


	inline Element<DataType,ndims,IndexPrecision>& bound(Index<ndims,IndexPrecision> idx)
	{
		Index<ndims,IndexPrecision> ret();
		IndexPrecision pos;
		for (auto dim = 0; dim != ndims; ++dim) {
			pos = bound_axis(idx[dim],);
			if (pos == -1) {
				return NONE;
			}
			ret[dim] = pos;
		}

		return this->operator()()
	}


    // Element setter/getter
    inline const Element<DataType, ndims, IndexPrecision>& operator()(const Index<ndims,IndexPrecision> index) {
		for (auto i = 0; i != ndims;++i) {
			// check if we're inside of the bounds
		}

        return elements[y*X+x];
    }
};


// Initialize the NONE type
template<class DataType, size_t ndims, class IndexPrecision>
Element<DataType, ndims, IndexPrecision> Grid<DataType,ndims,IndexPrecision>::NONE = Element<DataType, ndims, IndexPrecision>(-1,-1,0,false);


