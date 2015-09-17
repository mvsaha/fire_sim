#include "Fire.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
   
   const int y = 10;
   const int x = 10;
   Grid<char> landscape(y,x);
   Grid<double> accum_energy(y,x);
   
   
   Fire fire(landscape,accum_energy,{10,2,16},4);
   
   long i;
   landscape.sub2ind(10, 0, &i);
   
   vector<double> v{0.01,34,99};
   
   
   double* pv = v.data();
   
   cout<<pv<<endl;
   cout<<v[0]<<endl;
   cout<<*pv<<endl;
   cout<<*++pv<<endl;
   cout<<*++pv<<endl;
   
   return 0;
}