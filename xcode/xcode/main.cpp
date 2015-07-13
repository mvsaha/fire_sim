#include <iostream>
#include <vector>
#include <functional>
#include <array>
#include <algorithm>

//#include "Grid.h"

using namespace std;


template<class T,size_t n>
T dot(const std::array<T,n>& a, const std::array<T,n>& b){
    T temp(0);
    for(auto i=0;i!=n;++i){
        temp += a[i]*b[i];
    }
    return temp;
}


int main()
{
    
    const int ndims(5);
    
    std::array<double,5> a{1,2,3,4,5};
    std::array<double,5> b{1,2,3,4,5};
    
    cout<<dot(a,b)<<endl;
    
    
    cout<<"oh hai thar"<<endl;
}
