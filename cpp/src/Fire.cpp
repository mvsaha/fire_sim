#include "Fire.h"
using namespace std;

// Fire::Fire
// Construct a fire to burn
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
   for(auto& pix:active_burns)
   {
      burn_pixel(pix);
   }
   
   return false;
}


vector<long> Fire::burn_pixel(long pixel)
{
   vector<long> new_burns;
   
   // Loop through the pixels
   
   for(int y_iter)
   {
      if( y_iter > landtype.y )
      {
         
      }
   }
   
   return new_burns;
}



