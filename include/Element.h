#pragma once
#include "Neighborhood.h"
#include "Index.h"


// Forward declare Grid to make Element mutual friends
//template<size_t ndims, class DataType>
//class Grid;

// Represents an element of a 2D grid with a payload of DataType
template<class DataType, size_t ndims, size_t n_neighbors, class IndexPrecision>
class Element
{
public:

	Element(const Index<ndims,IndexPrecision>& index,
		const DataType& v,
		bool exists) :
		index(index), value(v), exists(exists) { }


    // Create Element from value
	Element(const Index<ndims,IndexPrecision>& index,
		const DataType& v,
		bool exists,
		const Neighborhood<n_neighbors>& neighborhood) :
	index(index), value(v), exists(exists), neighbors(neighborhood.n) { }

	// Location of Element
	const Index<ndims, IndexPrecision> index;

	// Raw pointers to neighbors
	std::vector<Element<DataType, ndims, n_neighbors, IndexPrecision>*> neighbors;

	const Index<ndims, IndexPrecision>& i() { return index; }
	DataType value; // The payload

    const bool exists;
    explicit operator bool() const { return exists; }
    
    
    void operator=(const DataType& newvalue) { value = newvalue; }
    
    // Return a reference to a neighbor
    inline const Element<DataType, ndims, n_neighbors, IndexPrecision>& neighbor(size_t n) const{
        return *(neighbors[n]); // Dereference the pointer inside
    }
    
    // Return a reference to a neighbor
    inline const Element<DataType, ndims, n_neighbors, IndexPrecision>& operator[](size_t n) const {
        return *(neighbors[n]);
    }
    
    void print() const {
        if (!exists) {
            cout <<"[NONE] ";
        }
        else{
			cout << "[";
			index.print();
			cout << ": " << value << "]";
		}
    }

	operator DataType() const {
		return value;
	}
};
