#include <iostream>
#include <vector>
//#include "Neighborhood.h"
#include <functional>
#include <array>


#include "Index.h"

using namespace std;


int main()
{
    // 2d interface for a resource managed vector
    //Grid<2,2,double,CONNECTIVITY::VON_NEUMANN> grid(0.3,false,true);
    
    const std::array<int,3> ind = {9,2,3};
    
    Index<3,int> i({3,4,1});
    Index<3,int> i2(ind);
    
    i.print();
    i2.print();
    
    
    cout<<endl<<i[2]<<endl;
    
    
    cout<<"oh hai thar"<<endl;
}
