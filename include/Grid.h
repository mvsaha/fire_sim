#pragma once

// Exception raised when bounds are invalid
class OutOfBoundsException : public std::exception{
	virtual const char* what() const throw(){
		return "Index exceeded Grid Size.";
	}
} OutOfBoundsException;




// Represents a two dimensional lattice
template<size_t Ysize, size_t Xsize, class DataType>
class Grid
{
	friend Element<Ysize, Xsize, DataType, N>;

public:
    
	// one 'invalid' object that all neighbor pointers will evaluate to
	static Element<Ysize, Xsize, DataType> NONE;
	
    Grid(DataType fill, bool yperiodic, bool xperiodic):
    X(Xsize), Y(Ysize), y_periodic(yperiodic),x_periodic(xperiodic)
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


