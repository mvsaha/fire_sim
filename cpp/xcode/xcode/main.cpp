#include "Fire.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
   
   Fire fire({10,2,16},4);
   
   cout<<fire.n_active_fires()<<endl;
   cout<<fire.n_active_fires()<<endl;
   return 0;
}