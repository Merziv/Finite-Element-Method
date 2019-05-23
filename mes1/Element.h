#pragma once
#include "GlobalNode.h"
#include "UniversalElement.h"
#include "Input.h"
#include <vector>
#include <array>

class Element
{
	unsigned long int id;
	array <GlobalNode*, 4> globalNodes;
	array <array <long double, 4>, 4> localH;
	array <array <long double, 4>, 4> localC;
	array <long double, 4> localP;
public:
	Element(unsigned long int id, array <GlobalNode*, 4> &globalNodes, UniversalElement universalELement, Input* input);
	~Element();

	unsigned long int getId();
	array <GlobalNode*, 4> &getGlobalNodes();
	array <array <long double, 4>, 4> &getLocalH();
	array <array <long double, 4>, 4> &getLocalC();
	array <long double, 4> &getLocalP();
};


