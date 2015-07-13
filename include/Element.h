#pragma once
#include "Neighborhood.h"
#include "Index.h"


// Forward declare Grid to make Element mutual friends
//template<size_t ndims, class DataType>
//class Grid;

// Represents an element of a 2D grid with a payload of DataType
template<class DataType, size_t ndims, class IndexPrecision>
class Element
{
public:

    // ctor - Private: only Grid will create/manage Elements
	Element(const Index<ndims, IndexPrecision>& index,
		const DataType& v,
		size_t n_neighbors,
		bool exists) :
		_index(index), value(v), exists(exists), neighbors(n_neighbors) { }

	Element(const Element<DataType, ndims, IndexPrecision>& e):
		_index(e._index),value(e.value),exists(e.exists),neighbors(e.neighbors.size())
	{ }
	
	// Location of Element
	const Index<ndims, IndexPrecision> _index;


	// Raw pointers to neighbors
	std::vector<Element<DataType, ndims, IndexPrecision>*> neighbors;

	const Index<ndims, IndexPrecision>& index() { return _index; }
	DataType value; // The payload

    const bool exists;
    explicit operator bool() const { return exists; }
    
    
    void operator=(const DataType& newvalue) { value = newvalue; }
    
    // Return a reference to a neighbor
    inline const Element<DataType, ndims, IndexPrecision>& neighbor(size_t n) const{
        return *(neighbors[n]); // Dereference the pointer inside
    }
    
    // Return a reference to a neighbor
    inline const Element<DataType, ndims, IndexPrecision>& operator[](size_t n) const {
        return *(neighbors[n]);
    }
    
    void print() const {
        if (!exists) {
            cout <<"[NONE] ";
        }
        else{
			cout << "[";
			_index.print();
			cout << ": " << value << "]";
		}
    }
};
