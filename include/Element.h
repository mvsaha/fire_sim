#pragma once

// Element must know
// Index[n] should have a value that is the




// Forward declare Grid to make Element mutual friends
//template<size_t ndims, class DataType>
//class Grid;


// Represents an element of a 2D grid with a payload of DataType
template<class DataType, Neighborhood& N, size_t ndims, class IndexPrecision>
class Element
{
    
    //friend class Grid<Ysize,Xsize,DataType>;
    
public:
    
    // ctor - Private: only Grid will create/manage Elements
    Element(size_t y, size_t ix, DataType v):
        y(y), x(x), value(v), exists(exists), neighbors(ndims)
    {
        
    }
    
    const Neighborhood& _neighborhood;
    
public:
    const bool exists; // Is this a valid Element, or will it exist as a dummy NONE element?
    explicit operator bool() const { return exists; } // Same thing as exists.
    
    // Change the value by directly assigning to it
    void operator=(const DataType& d) { value = d; }
    
    //
    const Index<ndims,IndexPrecision> index;
    
    // Raw Pointers to neighbors
    std::vector<Element<Ysize, Xsize, DataType, C>*> neighbors;
    
    // Return a reference to a neighbor
    inline Element< Ysize, Xsize, DataType, C >& neighbor(size_t n){
        return *(neighbors[n]); // Dereference the pointer inside
    }
    
    // Return a reference to a neighbor
    inline Element< Ysize, Xsize, DataType, C>& operator[](size_t n) {
        return *(neighbors[n]);
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
