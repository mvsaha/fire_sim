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
   std::vector<T> data;
   const int ysize;
   const int xsize;
   
   //===========================================//
   //               INLINE METHODS              //
   //===========================================//
   
   // Convert a linear index to y,x coordinates
   inline void ind2sub(const long i, int* y, int* x){
      *y = int(i/xsize);
      *x = int(i - ((*y) * ysize));
      return;
   }
   
   // Convert y,x coordinates to a linear index
   inline long sub2ind(const int y, const int x){
      return (y*xsize) + x;
   }
   
   // Convert y,x coordinates to a linear index
   inline void sub2ind(const int y, const int x, long* i){
      *i = (y*xsize) + x;
      return;
   }
   
   // Retrieve data at linear index.
   inline const T& operator()(long i) const{
      return data[i];
   };
   
   // Retrieve the pointer to the address of a given linear index
   inline T* ptr(long i){
      return &(data[i]);
   }
   
   // Retrieve data at subscripted index.
   inline const T& operator() (int y, int x) const{
      long i;
      sub2ind(y,x,&i);
      return data[i];
   }
};

template<class T>
Grid<T>::Grid(int y,int x):
xsize(x),ysize(y)
{
   //
}