#include <iostream>
#include "Input.h"
#include "Element.h"
#include "Grid.h"
using namespace std;

int main()
{
	Input input;
	Grid grid;

	grid.displayGrid();
	cout << endl;
	grid.calculate();

	system("pause");

	return 0;
}