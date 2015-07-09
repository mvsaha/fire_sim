#include <iostream>

#include "Grid.h"

using namespace std;

int main(int argc, const char * argv[])
{
    // 2d interface for a resource managed vector
    Grid<2,2,double,CONNECTIVITY::VON_NEUMANN> grid(0.3,false,true);
    
}
