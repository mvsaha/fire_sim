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
   int y;
   int x;
   
   //===========================================//
   //               METHODS                     //
   //===========================================//
   void ind2sub(long i, int* y, int*  x);
   void sub2ind(int  y,  int x, long* i);
   
   //===========================================//
   //               INLINE METHODS              //
   //===========================================//
   
   // Retrieve data at linear index.
   inline const& T operator () (long i)
   {
      return data[i];
   }
   
   // Retrieve data at subscripted index.
   inline const& T operator () (int y, int x)
   {
      return data
   }
};

void Grid::ind2sub(const long i, int* y, int* x)
{
   *y = i/self.x;
   *x = i - ((*y) * self.y);
   return
}

void Grid::sub2ind(const int y, const int x, long* i)
{
   *i = (y*self.x) + x;
   return
}

