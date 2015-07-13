#include <iostream>
//#include "Index.h"

#include "Element.h"
#include "Grid.h"

#include <array>

using namespace std;

int main(int argc, const char * argv[])
{
	std::array<long, 3> arr={ 1,2,3 };

	const int ndims(3);
	Index<ndims> i(arr);
	Index<ndims> j({ -1,-3,-5});

	auto v = 9899.3124;

	Element<double,ndims,long> e(i, v, MooreNeighborhood.n, true);
	Element<double, ndims, long> e_(i, v, MooreNeighborhood.n, false);

	std::array<long, ndims> dimensions{2,3,2};
	std::array<BOUNDARY, ndims> boundaries{ BOUNDARY::PERIODIC,BOUNDARY::PERIODIC,BOUNDARY::PERIODIC };

	Grid<double, ndims, long> grid(dimensions, boundaries, MooreNeighborhood, 0.2233);

	cout << grid.elements.size() << endl;
	cin.ignore();
}
