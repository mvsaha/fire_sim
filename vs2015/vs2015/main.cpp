#include <iostream>
#include <string>
#include <array>

using namespace std;

#include "Neighborhood.h"
#include "Grid.h"




int main(int argc, const char * argv[])
{
	const int n_dimensions = 2;
	const int n_neighbors =  4;

	std::array<long,n_dimensions> dimensions{ 3,4 };
	std::array<BOUNDARY, n_dimensions> boundaries{ BOUNDARY::PERIODIC, BOUNDARY::NONPERIODIC };
	Grid<double, n_dimensions, n_neighbors, long> grid(dimensions, boundaries, VonNeumannNeighborhood, 0);

	grid(2, 1) = 213;

	auto i = grid(0, 0).i();
	for (auto y = 0; y != dimensions[0]; ++y) {
		for (auto x = 0; x != dimensions[1]; ++x) {
			auto& e = grid(y, x);
			e.print();
		}
		cout << endl;
	}
	//e.print();
	cin.ignore();
}
