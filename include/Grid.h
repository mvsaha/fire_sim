#pragma once

#include <iostream>

using namespace std;


#include <vector>
#include <stdexcept>

// How are pixels connected?
enum class CONNECTIVITY
{
    VON_NEUMANN = 4,
    MOORE = 8
};


//

enum class NEIGHBOR
{
    L=0,
    U=1,
    R=2,
    D=3,
    UL=4,
    UR=5,
    LR=6,
    LL=7,
};


// Represents an element into the 2d lattice
// Templated to a certain grid size and DataType
template<class Ysize, class Xsize,class DataType, CONNECTIVITY C>
class Element
{
public:
    // ctor
    Element(size_t y, size_t x):y(y),x(x){}
    Element(size_t y, size_t x,DataType v):y(y),x(x),value(v){
        neighbors.resize(4);
    }
    
    const size_t y;
    const size_t x;
    
    // Reference counted pointers to neighbors
    // Should be indexed with a NEIGHBOR enum, or a range-based for loop
    std::vector<std::weak_ptr<Element<Ysize, Xsize, DataType, C>>> neighbors;
    
    // Return a neighbor
    inline std::weak_ptr<Element<Ysize,Xsize,DataType,C>> neighbor(NEIGHBOR n){
        return neighbors[n]; // Dereference the pointer inside
    }
    
    DataType value; // Payload
};



class OutOfBoundsException: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Index exceeded Grid Size.";
    }
} OutOfBoundsException;


// Represents a two dimensional lattice
template<size_t Ysize, size_t Xsize, class DataType, CONNECTIVITY C>
class Grid
{
public:
    
    Grid(DataType fill, bool yperiodic, bool xperiodic):
    X(Xsize), Y(Ysize), yper(yperiodic),xper(xperiodic), conn(C)
    {
        elements.reserve(Y*X);
        
        // Build elements
        for (size_t iy=0; iy!=Y;++iy){
            for (long ix=0;ix!=X;++ix){
                elements.emplace_back(iy,ix,fill);
            }
        }
        
        cout<<elements.size()<<endl;
        
        // Link elements
        for (size_t iy=0; iy!=Y;++iy){
            for (size_t ix=0;ix!=X;++ix){
                auto el = elements[iy*X+ix];
                
                cout<<el.value;
                
                
                //cout<<el.y<<" "<<el.x<<" ***  ";
                /*for(auto i:el.neighbors){
                    auto shared = i.lock();
                    cout<<shared<<"  ";
                }*/
                
                cout<<endl;
            }
        }
    }
    
    const CONNECTIVITY conn;
    const size_t Y;
    const size_t X;
    
    // Element setter/getter
    inline Element<size_t,size_t,DataType,C>& operator()(size_t y,size_t x) {
        if ( y>=Y || y<0 || x>=X || x<0) {
            throw OutOfBoundsException;
        }
        return elements[y*X+x];
    }
    
    
//private:
    
    // Data
    std::vector<Element<size_t,size_t,DataType,C>> elements;
    const bool yper = false;
    const bool xper;
    
};

