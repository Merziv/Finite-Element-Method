#pragma once
#include <array>
using namespace std;

class UniversalElement
{
	array <array <long double, 4>, 4> N;
	array <array <long double, 4>, 8> NSurface;
	array <array <long double, 4>, 4> dNdEta;
	array <array <long double, 4>, 4> dNdKsi;

public:
	UniversalElement();
	~UniversalElement();

	array <array <long double, 4>, 4> &getN();
	array <array <long double, 4>, 8>  &getNSurface();
	array <array <long double, 4>, 4> &getdNdEta();
	array <array <long double, 4>, 4> &getdNdKsi();
};

