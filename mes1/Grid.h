#pragma once
#include "GlobalNode.h"
#include "Element.h"
#include "UniversalElement.h"
#include <vector>

class Grid
{
	Input input;
	vector <GlobalNode*> nodes;
	vector <Element*> elements;
	UniversalElement universalElement;

public:
	Grid();
	~Grid();

	void calculate();
	void displayGrid();
};

