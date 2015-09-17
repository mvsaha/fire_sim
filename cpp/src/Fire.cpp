#include "Fire.h"
#include <algorithm>
#include <cmath>

using namespace std;


Fire::Fire(Grid<char> landtype, Grid<double> accum_energy, vector<long>&& ignitions, int max_radius):
active_burns(ignitions),
max_radius(max_radius),
landtype(landtype),
accum_energy(accum_energy)
{
   //
}


bool Fire::iterate()
{
   for(auto& pix:active_burns){
      burn_pixel(pix);
   }
   return false;
}


// Find the Euclidean distance between two points
inline double distance(int y0,int x0,int y1,int x1)
{
   return sqrt(((y0-y1)*(y0-y1)) + ((x0-x1)*(x0-x1)));
}


// Apply burn energy from a burning pixel to neighbors
inline vector<long> Fire::burn_pixel(long pixel,double released_energy)
{
   vector<long> new_burns;
   
   int x,y;
   landtype.ind2sub(pixel,&y,&x);
   
   int ybeg = std::max(y-max_radius,0);
   int yend = std::min(y+max_radius+1,landtype.ysize);
   int xbeg = std::max(x-max_radius,0);
   int xend = std::min(x+max_radius+1,landtype.xsize);
   
   
   long i;      // Linear index for the pixel
   double dist; // distance from the current pixel
   double* ptr; // Pointer
   
   // Loop through the pixels and check that they are within max radius
   for(int iy=ybeg;y!=yend;++iy){
      i = landtype.sub2ind(iy, xbeg);
      for(int ix=xbeg;x!=xend;++x,++ptr){
         dist = distance(y,x,iy,ix);
         if (dist > max_radius){
            continue;
         }
         else {
            (*ptr) += released_energy;
         }
      }
   }
   return new_burns;
}



