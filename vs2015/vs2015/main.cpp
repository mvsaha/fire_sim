#include <iostream>
#include "Grid.h"

using namespace std;

int main(int argc, const char * argv[])
{
	// 2D Lattice
	Grid<5, 5, double, CONNECTIVITY::VON_NEUMANN> grid(0.333f, true, true);
	/*
	//cout << grid.elements.size() << endl;
	cout << grid(1, 0).value << endl;

	auto el = grid.NONE;

	cout << el.value << endl;
	cout << el.y << " " << el.x << endl;
	cout << bool(el) << endl;
	*/
	
	cout << endl;
	auto e = grid(0, 4);

	cin.ignore();
	e.print();
	auto n = e.neighbor();
	
	if (!n) {
		cout << "none" << endl;
	}
	else {
		n.print();
	}
	

	cin.ignore();
}
