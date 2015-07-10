#pragma once

#include <iostream>
#include <memory>
#include <assert.h>
#include <vector>
#include <stdexcept>

using namespace std;


// How are pixels connected?
enum class CONNECTIVITY
{
    VON_NEUMANN = 4,
    MOORE = 8
};



// Relative offsets
static const long long y_neighbors[]{ 0,-1,0,1,-1,-1,1,1 };
static const long long x_neighbors[]{ -1,0,1,0,-1,1,1,-1 };


// Exception raised when bounds are invalid
class OutOfBoundsException : public std::exception{
	virtual const char* what() const throw(){
		return "Index exceeded Grid Size.";
	}
} OutOfBoundsException;


// Forward declare Grid to make Element mutual friends
template<size_t Ysize, size_t Xsize, class DataType, CONNECTIVITY C>
class Grid;


// Represents an element of a 2D grid with a payload of DataType
template<size_t Ysize, size_t Xsize,class DataType, CONNECTIVITY C>
class Element{

	friend class Grid<Ysize,Xsize,DataType,C>;

private:

	// ctor - Private: only Grid will create/manage Elements
    Element(size_t y, size_t x,DataType v,bool exists=true):
		y(y),x(x),value(v),exists(exists)
	{
        neighbors.resize(static_cast<long>(C));
    }
    
public:
	const bool exists;
	explicit operator bool() const { return exists; }

	void operator=(const DataType& d) { value = d; }
	
    const long long y;
    const long long x;
    
    // Raw Pointers to neighbors
    std::vector<Element<Ysize, Xsize, DataType, C>*> neighbors;
    
    // Return a reference to a neighbor
    inline Element< Ysize, Xsize, DataType, C >& neighbor(size_t n){
        return *(neighbors[n]); // Dereference the pointer inside
    }

	// Return a reference to a neighbor
	inline Element< Ysize, Xsize, DataType, C>& operator[](size_t n) {
		return *(neighbors[n])
	}

	void print() const {
		if (!exists) {
			cout <<"[NONE] ";
		}
		else{
			cout <<"[("<<y<<","<<x<<"): " <<value<<"] ";
		}
	}
    
    DataType value; // The payload

private:
	template<class T>
	inline void operator=(T d) {/* Do nothing;*/ }
};


// Represents a two dimensional lattice
template<size_t Ysize, size_t Xsize, class DataType, CONNECTIVITY C>
class Grid
{
	friend Element<Ysize, Xsize, DataType, C>;

public:
    
	// one 'invalid' object that all neighbor pointers will evaluate to
	static Element<Ysize, Xsize, DataType, C> NONE;
	
    Grid(DataType fill, bool yperiodic, bool xperiodic):
    X(Xsize), Y(Ysize), y_periodic(yperiodic),x_periodic(xperiodic), conn(C)
    {
        elements.reserve(Y*X);
        
        // Build elements
        for (size_t iy=0; iy!=Y;++iy){
            for (long ix=0;ix!=X;++ix){

				//TODO: figure out how to emplace_back an Element
				// while preserving its private ctor
				Element<Ysize, Xsize, DataType, C> el(iy,ix,fill,true);
                elements.push_back(el);
            }
        }

		link_elements();
    }

	// Assign neighbors
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
	}

	
	inline const long bound_axis(long val, long B, bool roll) const {
		// Returns -1 if not in range [0,B) and roll is false
		// Otherwise returns bounded value.
		if (val >= B) {
			if (roll) {
				return val%B;}
			else {
				return -1;}
		}
		else if (val < 0) {
			if (roll) {
				val *= -1; // val must be positive now
				return B - (val%B);
			}
			else {
				return -1;}
		}
		return val;
	}


	inline Element<Ysize,Xsize,DataType,C>& bound(long y, long x) {
		auto _y = bound_axis(y, Y, y_periodic);
		auto _x = bound_axis(x, X, x_periodic);
		if (_y >= 0 && _x >= 0) {
			return this->operator()(_y, _x);}
		else {
			return NONE;}
	}


	// Properties (derived, probably could get rid of these and cast from)
    const CONNECTIVITY conn;
    const size_t Y;
    const size_t X;
    

    // Element setter/getter
    inline Element<Ysize,Xsize,DataType,C>& operator()(size_t y,size_t x) {
        if ( y>=Y || y<0 || x>=X || x<0 ) {
            throw OutOfBoundsException;
        }
        return elements[y*X+x];
    }
    

	const bool y_periodic;
	const bool x_periodic;
	
private:

    // Data
    std::vector<Element<Ysize,Xsize,DataType,C>> elements;
    
};


// Initialize the NONE type
template<size_t Ysize, size_t Xsize, class DataType, CONNECTIVITY C>
Element<Ysize,Xsize,DataType,C> Grid<Ysize, Xsize, DataType, C>::NONE = Element<Ysize, Xsize, DataType, C>(-1,-1,0,false);


