#include <iostream>
#include "Grid.h"

#include <string>
#include <array>

#include <chrono>

using namespace std;

int main(int argc, const char * argv[])
{
	const int ndims = 2;

	std::array<long, ndims> dimensions{1000,1000};
	std::array<BOUNDARY, ndims> boundaries{ BOUNDARY::PERIODIC,BOUNDARY::PERIODIC};

	Grid<double, ndims, 0, long> grid(dimensions, boundaries, LonelyNeighborhood, 0.0);
	//Grid<double, ndims, 4, long> grid(dimensions, boundaries, MooreNeighborhood, 0.0);

	grid(1202) = 23.0;

	cout << grid(1201) << " " << grid(1202) << " " << grid(1203) << endl;

	cin.ignore();
}
