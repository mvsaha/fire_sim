#include "Fire.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
   
   const int y = 10;
   const int x = 10;
   
   Grid<char> landscape(y,x);
   Grid<double> activation_energy(y,x);
   Grid<double> released_energy(y,x);
   Grid<double> accumulated_energy(y,x);
   
   Fire fire(landscape,
             activation_energy,
             released_energy,
             accumulated_energy,
             {10,2,16},
             4);
   
   long i = 10;
   
   cout << activation_energy[i] <<endl;
   activation_energy[i] = 99;
   cout << activation_energy[i] <<endl;
   
   int ix,iy;
   landscape.ind2sub(i, &iy, &ix);
   cout<< activation_energy(iy,ix)<<endl;
   
   return 0;
}