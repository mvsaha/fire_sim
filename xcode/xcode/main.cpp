#include <iostream>
#include <vector>
#include <functional>
#include <array>
#include <algorithm>

#include "Neighborhood.h"
#include "Index.h"

using namespace std;



int main()
{
    Index<3,long> i(10);
    i.print();
    cout<<endl;
    
    
    
    Neighborhood x = LonelyNeighborhood;
    cout<<x.offsets.size()<<endl;
}
