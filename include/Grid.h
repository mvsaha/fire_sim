#pragma once
#include <chrono>
#include "Element.h"

#include <type_traits>

enum class BOUNDARY {
	NONPERIODIC,
	PERIODIC,
};


// Dot product of two identically sized arrays
template<class T,size_t n>
inline size_t dot(const std::array<T,n>& a, const std::array<T,n>& b){
	static_assert(std::is_integral<T>::value,"Dot only works on integral types.");
	// Templated function for numerical types 
    size_t temp(0);
    for(auto i=0;i!=n;++i){
        temp += a[i]*b[i];
    }
    return temp;
}


// Represents a n-dimensional array
template<class DataType,size_t ndims, size_t n_neighbors, class IndexPrecision>
class Grid
{
public:
	friend Element<DataType, ndims, n_neighbors, IndexPrecision>;

	//========================================================
	//                      Constructor
	//========================================================
	Grid(const std::array<IndexPrecision, ndims>& dimensions,
		const std::array<BOUNDARY, ndims>& boundaries,
		const Neighborhood<n_neighbors>& neighborhood,
		const DataType& fill) :
	dimensions(dimensions),boundaries(boundaries),neighborhood(neighborhood), NONE(Index<ndims, IndexPrecision>(IndexPrecision(0)), fill, false, neighborhood)
	{
		static_assert(ndims > 0,"Must have at least one dimension.");

		dim_stride[ndims - 1] = 1;
		for (auto dim = ndims - 2; dim != -1; --dim) {
			dim_stride[dim] = dim_stride[dim + 1] * dimensions[dim + 1];
		}

		chrono::time_point<std::chrono::high_resolution_clock> t_start, t_end;
		t_start = std::chrono::high_resolution_clock::now();

		populate_elements(fill); // Heavy lifting (most of the time is spent extending the vectors)...
		link_elements();

		t_end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = t_end - t_start;

		Index<ndims, IndexPrecision> size_str(dimensions);
		cout << "Building ";
		size_str.print();
		cout << " Grid took " << elapsed.count() << " seconds." << endl;
	}

	//========================================================
	//                      Properties
	//========================================================
	Neighborhood<n_neighbors> neighborhood;
	std::vector<Element<DataType, ndims, n_neighbors, IndexPrecision>> elements;

	const std::array<IndexPrecision, ndims>& dimensions;
	const std::array<BOUNDARY, ndims>& boundaries;
	const Element<DataType, ndims, n_neighbors, IndexPrecision> NONE;

	// dim_stride[dim] is the number of elements we transverse if we
	// increment the index of a given dim.
	// dim_stride[ndims-1] is defined as 1,indicating ROW-MAJOR (C-style)
	// storage. dim_stride monotonically decreases with increasing dim.
	std::array<IndexPrecision, ndims> dim_stride;


	//========================================================
	//                      Methods
	//========================================================

	// Build the elements
	void populate_elements(const DataType& fill)
	{
		Index<ndims, IndexPrecision> idx;
		IndexPrecision n_elements = dim_stride[0] * dimensions[0];
		elements.reserve(n_elements);

		cout << "Populating: ";
		for (IndexPrecision i = 0; i != n_elements; ++i) {
			if (i % 1000000 == 0) {
				cout << i / 1E6 << "M ";
			}
			elements.emplace_back(lindex_to_index(i), fill, true, neighborhood);
		}
		cout << endl;
	};


	// Set the neighborhoods
	void link_elements()
	{
		std::vector<int> relevant_dim_inds;
		std::vector<int> relevant_dims;

		for (IndexPrecision i = 0; i != neighborhood.dims.size(); ++i) {
			if (i < ndims) {
				relevant_dim_inds.push_back(i);
				relevant_dims.push_back(dimensions[i]);
			}
		}

		cout << "Linking: ";
		IndexPrecision i(0);
		for (auto& e : elements) {
			if (i % 1000000 == 0) { cout << i / 1E6 << "M "; }
			for (IndexPrecision n = 0; n != neighborhood.n; ++n) {
					
				for (int dim = 0; dim != relevant_dims.size(); dim++) {
					Index<ndims, IndexPrecision> idx;
				}

			}
			++i;
		}
		cout << endl;
	}


	// Convert a linear index to its corresponding Index object
	inline Index<ndims, IndexPrecision> lindex_to_index(IndexPrecision i)
	{
		Index<ndims, IndexPrecision> idx;
		IndexPrecision remainder(elements.size());
		for (IndexPrecision dim = 0; dim != ndims; ++dim) {
			idx[dim] = remainder / dim_stride[dim];
			remainder -= idx[dim] * dim_stride[dim];
		}
		return idx;
	}


	// Bound a index value for a given dimension
	inline const IndexPrecision bound_axis(IndexPrecision val, int dim)
		const {
		// Returns -1 if val is not in range [0,size_of_dim) and the dimension
		// is not periodic.
		// Otherwise it just returns bounded val.
		IndexPrecision B(dimensions[dim]);

		if (val >= B) {
			if (boundaries[dim] == BOUNDARY::PERIODIC) {
				return val%B;
			}
			else {
				return -1;
			}
		}
		else if (val < 0) {
			if (boundaries[dim] == BOUNDARY::PERIODIC) {
				val *= -1; // val must be positive now
				return B - (val%B);
			}
			else {
				return -1;
			}
		}
		return val;
	}


	// Given an Index, return the Element associated with that index
	inline Element<DataType, ndims, n_neighbors, IndexPrecision>& bound(Index<ndims, IndexPrecision> idx)
	{
		Index<ndims, IndexPrecision> ret(IndexPrecision(0));
		IndexPrecision pos;
		for (IndexPrecision dim = 0; dim != ndims; ++dim) {
			pos = bound_axis(idx[dim], dim);
			if (pos == -1) {
				return NONE;
			}
			ret[dim] = pos;
		}
		return this->operator()(ret);
	}


	// Set 
	inline void set_neighbors(const Index<ndims, IndexPrecision>& idx) {
		auto offset_idx(idx);
		auto& e = this->operator()(idx);

		auto& dims = neighborhood.dims;
		auto& offsets = neighborhood.offsets;

		// Loop through neighbors
		for (auto n = 0; n != n_neighbors;++n) {

			// Loop through dimensions of neighborhood
			for (auto idim = 0; idim) {
					
			}
				


		}
	}

	// Element getter/setter
	inline Element<DataType, ndims, n_neighbors, IndexPrecision>& operator()(const Index<ndims, IndexPrecision>& index) {
		return elements[dot(index.indices, dim_stride)];
	}


	// Linear index Element getter/setter
	inline Element<DataType, ndims, n_neighbors, IndexPrecision>& operator()(const size_t& idx) {
		return elements[idx];
	}


	// Variadic indices
	template<class... I>
	inline Element<DataType, ndims, n_neighbors, IndexPrecision>& operator()(IndexPrecision ind1, I&&... inds) {
		static_assert(sizeof...(inds) == (ndims - 1),"Must supply ndims indices.");
		return this->operator()(Index<ndims, IndexPrecision>(ind1,std::forward<I>(inds)...));
	}
};