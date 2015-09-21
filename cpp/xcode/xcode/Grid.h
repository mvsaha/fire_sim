#pragma once
#include <vector>

/// Lightweight wrapper around a std::vector as a 2d array
template<class T>
class Grid
{
public:
   //===========================================//
   //               CONSTRUCTORS                //
   //===========================================//
   Grid(int y,int x);
   
   
   //===========================================//
   //               PROPERTIES                  //
   //===========================================//
   
   /// Data that is wrapped
   std::vector<T> data;
   
   /// Constant dimensions of the grid
   const int ysize;
   const int xsize;
   
   
   //===========================================//
   //               INLINE METHODS              //
   //===========================================//
   
   /// Convert a linear index to y,x coordinates
   inline void ind2sub(const long i, int* y, int* x){
      *y = int(i/xsize);
      *x = int(i - ((*y) * ysize));
      return;
   }
   
   /// Convert y,x coordinates to a linear index
   inline long sub2ind(const int y, const int x){
      return (y*xsize) + x;
   }
   
   /// Convert y,x coordinates to a linear index
   inline void sub2ind(const int y, const int x, long* i){
      *i = (y*xsize) + x;
      return;
   }
   
   //===========================================//
   //               GETTERS                     //
   //===========================================//
   // All getters return a reference to the underlying value which can
   // be used to change the underlying value.
   
   /// Retrieve data at a linear index.
   inline T& operator[](long i) {
      return data[i];
   };
   
   /// Retrieve data at a linear index.
   inline T& operator()(long i) {
      return data[i];
   };
   
   /// Retrieve the pointer to the address of a given linear index
   inline T* ptr(long i){
      return &(data[i]);
   }
   
   /// Retrieve data at subscripted index.
   inline T& operator() (int y, int x) {
      long i;
      sub2ind(y,x,&i);
      return data[i];
   }
   
   //===========================================//
   //               UTILS                       //
   //===========================================//
   inline size_t size(){
      return xsize*ysize;
   }
   // 
   
};

// Constructor
template<class T>
Grid<T>::Grid(int y,int x):
data(x*y),
xsize(x),ysize(y)
{
   
}