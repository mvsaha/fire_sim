#pragma once

#include <iostream>
#include <vector>


// Exception raised when axes don't align but should
class DimensionException : public std::exception{
    virtual const char* what() const throw(){
        return "Input dimensions.";
    }
} AlignmentException;


template<size_t ndims,class IndexPrecision>
class Index
{
public:
    
    // ctor
    Index(const std::array<IndexPrecision,ndims>& indices):
        indices(indices){
    }
    
    
    // Data
    std::array<IndexPrecision,ndims> indices;
    
    // Accessors
    //const:
    const IndexPrecision& operator[](const size_t& dim) const {
        return indices[dim];
    }
    
    // non-const:
    IndexPrecision& operator[](const size_t& dim){
        return indices[dim];
    }
    
    // display
    void print() const {
        std::cout<<"[";
        for(auto i=0;i!=ndims;++i){
            std::cout<<" "<<indices[i];
        }
        std::cout<<" ]";
    }
    
};
