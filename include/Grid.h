#pragma once
#include <chrono>
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


// Dot product of two identically sized arrays
template<class T,size_t n>
inline T dot(const std::array<T,n>& a, const std::array<T,n>& b){
	// Templated function for numerical types 
    T temp(0);
    for(auto i=0;i!=n;++i){
        temp += a[i]*b[i];
    }
    return temp;
}

// Dot product of two identically sized arrays
template<class T, size_t n>
inline T dot(const std::initializer_list<T>& a, const std::array<T, n>& b) {
	// Templated function for numerical types 
	T temp(0);
	for (auto i = 0; i != n; ++i) {
		temp += a[i] * b[i];
	}
	return temp;
}

// Represents a n-dimensional lattice
template<class DataType,size_t ndims, size_t n_neighbors, class IndexPrecision>
class Grid
{
public:
	friend Element<DataType,ndims,n_neighbors,IndexPrecision>;

	Neighborhood<n_neighbors> neighborhood;
	std::vector<Element<DataType,ndims,n_neighbors,IndexPrecision>> elements;
	static Element<DataType,ndims,n_neighbors,IndexPrecision> NONE;
	const std::array<IndexPrecision, ndims>& dimensions;
	const std::array<BOUNDARY, ndims>& boundaries;

	// dim_stride[dim] is the number of elements we transverse if we
	// increment the index of a given dim.
	// dim_stride[ndims-1] is defined as 1,indicating ROW-MAJOR (C-style)
	// storage. dim_stride monotonically decreases with increasing dim.
	std::array<IndexPrecision,ndims> dim_stride;

    Grid(const std::array<IndexPrecision,ndims>& dimensions,
		 const std::array<BOUNDARY,ndims>& boundaries,
		 const Neighborhood<n_neighbors>& neighborhood,
		 const DataType& fill=0):
	dimensions(dimensions),
	boundaries(boundaries),
	neighborhood(neighborhood)
    {
		assert(ndims > 0);
		assert(ndims == dimensions.size()); // Inputs must match

		dim_stride[ndims - 1] = 1;
		for (auto dim = ndims - 2; dim != -1; --dim) {
			dim_stride[dim] = dim_stride[dim+1] * dimensions[dim+1];
		}
		
		chrono::time_point<std::chrono::high_resolution_clock> t_start, t_end;
		t_start = std::chrono::high_resolution_clock::now();

		populate_elements(fill); // Heavy lifting...
		link_elements();

		t_end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = t_end - t_start;

		Index<ndims,IndexPrecision> size_str(dimensions);
		cout << "Populating "; 
		size_str.print();
		cout<<" Grid took "<< elapsed_seconds.count() <<" seconds."<<endl;
    }

private:
	void populate_elements(const DataType& fill)
	{
		Index<ndims,IndexPrecision> idx;
		IndexPrecision n_elements = dim_stride[0] * dimensions[0];
		elements.reserve(n_elements);
		for (IndexPrecision i = 0; i != n_elements; ++i){
			if (i % 1000000 == 0) {
				cout << i/1E6 << "M ";
			}
			elements.emplace_back(lindex_to_index(i), fill, true,neighborhood);
		}
	};


	void link_elements()
	{
		cout << "Linking." << endl;
		//Loop over elements
		//Element<DataType,ndims,IndexPrecision>&

		// Extract relevant dimensions and their position in
		// neighborhood.offsets
		std::vector<int> relevant_dim_inds;
		std::vector<int> relevant_dims;

		for (auto i = 0; i != neighborhood.dims.size(); ++i) {
			if (i < ndims) {
				relevant_dim_inds.push_back(i);
				relevant_dims.push_back(dims[i]);
			}
		}
		
		for (auto e : elements) {
			assert(e.neighbors.size() == neighborhood.n);
			//
			for (auto n = 0; n != neighborhood.n; ++n) {
				// Calculate index after ofset
				// If it is valid (with periodicity) then assign it...

				for (int dim = 0; dim != relevant_dims.size(); dim++) {
					Index<ndims, IndexPrecision> idx({ 0,0,0 });
				}

				neighborhood.offsets[n]
				e.neighbors[n] = &NONE;
			}
		}
	}


	inline Index<ndims,IndexPrecision> lindex_to_index(IndexPrecision i)
	{
		Index<ndims, IndexPrecision> idx;

		IndexPrecision remainder(elements.size());
		for (auto dim = 0; dim != ndims; ++dim) {
			idx[dim] = remainder / dim_stride[dim];
			remainder -= idx[dim]*dim_stride[dim];
		}
		return idx;
	}
	

	inline const IndexPrecision bound_axis(IndexPrecision val, int dim)
	const {
		// Returns -1 if val is not in range [0,size_of_dim) and the dimension
		// is not periodic.
		// Otherwise it just returns bounded val.
        IndexPrecision B(dimensions[dim]);
        
		if (val >= B) {
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


	inline Element<DataType,ndims,n_neighbors,IndexPrecision>& bound(Index<ndims,IndexPrecision> idx)
	{
        Index<ndims,IndexPrecision> ret({0,0,0});
		IndexPrecision pos;
		for (auto dim = 0; dim != ndims; ++dim) {
			pos = bound_axis(idx[dim],dim);
			if (pos == -1) {
				return NONE;
			}
			ret[dim] = pos;
		}

        return this->operator()(ret);
	}


public:
    // Element setter/getter
    inline Element<DataType, ndims, n_neighbors, IndexPrecision>& operator()(const Index<ndims,IndexPrecision> index) {
        return elements[dot(index.indices,dim_stride)];
    }
	
	inline Element<DataType, ndims, n_neighbors, IndexPrecision>& operator()(const size_t& idx) {
		return elements[idx];
	}

};


// Initialize the NONE type
template<class DataType, size_t ndims, size_t n_neighbors, class IndexPrecision>
Element<DataType, ndims, n_neighbors IndexPrecision> Grid<DataType, ndims, n_neighbors,IndexPrecision>::NONE(Index<ndims, IndexPrecision>(-1),-1, false, LonelyNeighborhood<n_neighbors>);


