#include "Fire.h"
#include <algorithm>
#include <cmath>

using namespace std;



Fire::Fire(
   Grid<char>& landtype,
   Grid<double>& activation_energy,
   Grid<double>& released_energy,
   Grid<double>& accumulated_energy,
   vector<long>&& ignitions,
   int max_radius
   ):
active_burns(ignitions),
max_radius(max_radius),
landtype(landtype),
AE(activation_energy),
RE(released_energy),
E(accumulated_energy)
{
   
}


// Iterate over all of the actively burning pixels
bool Fire::step()
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
inline vector<long> Fire::burn_pixel(long burning_pixel)
{
   vector<long> new_burns;
   
   int x,y;
   landtype.ind2sub(burning_pixel,&y,&x);
   
   int ybeg = std::max(y-max_radius,0);
   int yend = std::min(y+max_radius+1,landtype.ysize);
   int xbeg = std::max(x-max_radius,0);
   int xend = std::min(x+max_radius+1,landtype.xsize);
   
   long lindex;      // Linear index for the looped pixel
   double dist; // distance from the current pixel
   double* ptr; // Pointer
   
   // Loop through the surrounding pixels
   for(int iy=ybeg;y!=yend;++iy){
      lindex = landtype.sub2ind(iy, xbeg);
      for(int ix=xbeg;  x!=xend;  ++x, ++ptr, ++lindex){
         dist = distance(y,x,iy,ix);
         if (dist > max_radius){
            continue;
         }
         else {
            AE[lindex] += RE[burning_pixel]/(dist*dist);
            if ( *ptr >= E[lindex] ){
               new_burns.emplace_back(lindex);
            }
         }
      }
   }
   return new_burns;
}



