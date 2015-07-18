#pragma once
#include "Neighborhood.h"
#include "Index.h"


// Forward declare Grid to make Element mutual friends
template<class DataType, size_t ndims, size_t n_neighbors, class IndexPrecision>
class Grid;

// Represents an element of a 2D grid with a payload of DataType
template<class DataType, size_t ndims, size_t n_neighbors, class IndexPrecision>
class Element
{
public:
    friend Grid<DataType,ndims,n_neighbors,IndexPrecision>;
    
	//========================================================
	//                      Constructors
	//========================================================
    // Create without neighbors
	Element(const Index<ndims,IndexPrecision>& index,
		const DataType& v,
		bool exists) :
	index(index), value(v), exists(exists) { }

    // Create with neighbors
	Element(const Index<ndims,IndexPrecision>& index,
		const DataType& v,
		bool exists,
		const Neighborhood<n_neighbors>& neighborhood) :
	index(index), value(v), exists(exists) { }

    
	//========================================================
	//                      Properties
	//========================================================

	// Location of the Element in the Grid
	const Index<ndims, IndexPrecision> index;

	// Raw pointers to neighbors
	std::array<Element<DataType, ndims, n_neighbors, IndexPrecision>*,n_neighbors> neighbors;

	DataType value; // The payload
    const bool exists; // Does this represent an element in the grid
	

    //========================================================
	//                      Accessors
	//========================================================
    inline explicit operator bool() const
		{ return exists; }

    inline const Index<ndims, IndexPrecision>& i()
		{ return index; }

    inline void operator=(const DataType& newvalue)
		{value = newvalue; }
    
    
	//========================================================
	//                 Neighborhood Accessors
	//========================================================
    // Return a reference to a neighbor
	inline const Element<DataType, ndims,n_neighbors, IndexPrecision>& neighbor(size_t n) const
		{return *(neighbors[n]);} // Dereference the pointer inside
    
    // Return a reference to a neighbor
    inline const Element<DataType, ndims,n_neighbors, IndexPrecision>& operator[](size_t n) const
		{return *(neighbors[n]);}
    
    
	//========================================================
	//                      Display
	//========================================================
    void print() const {
        if (!exists)
			{cout <<"[NONE] ";}
        else
			{cout << "[";index.print();cout << ": " << value << "]";}
    }
    
	
};
