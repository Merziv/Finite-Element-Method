#pragma once
#include "Input.h"

class GlobalNode
{
	unsigned long int id;
	long double x;
	long double y;
	long double t;
	bool onBound;
public:
	GlobalNode(unsigned long int id, long double x, long double y, bool onBound, Input *input);
	~GlobalNode();

	unsigned long int getId();
	long double getX();
	long double getY();
	long double getT();
	bool getOnBound();

	void setT(long double t);
};

