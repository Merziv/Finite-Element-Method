#include "UniversalElement.h"
#include <cmath>

UniversalElement::UniversalElement()
{
	long double val = 1 / sqrt(3);
	long double ksi[] = { val*(-1), val, val, val*(-1) };
	long double eta[] = { val*(-1), val*(-1), val, val };

	for (int i = 0; i < 4; i++)
	{
		N[i][0] = 0.25*(1 - ksi[i])*(1 - eta[i]);
		N[i][1] = 0.25*(1 + ksi[i])*(1 - eta[i]);
		N[i][2] = 0.25*(1 + ksi[i])*(1 + eta[i]);
		N[i][3] = 0.25*(1 - ksi[i])*(1 + eta[i]);

		dNdKsi[i][0] = (-0.25)*(1 - ksi[i]);
		dNdKsi[i][1] = (-0.25)*(1 + ksi[i]);
		dNdKsi[i][2] = (0.25)*(1 + ksi[i]);
		dNdKsi[i][3] = 0.25*(1 - ksi[i]);

		dNdEta[i][0] = (-0.25)*(1 - eta[i]);
		dNdEta[i][1] = (0.25)*(1 - eta[i]);
		dNdEta[i][2] = (0.25)*(1 + eta[i]);
		dNdEta[i][3] = (-0.25)*(1 + eta[i]);
	}

	long double ksiSurface[] = { -val, val, 1, 1, val, -val, -1, -1 };
	long double etaSurface[] = { -1, -1, -val, val, 1, 1, val, -val };

	for (int i = 0; i < 8; i++)
	{
		NSurface[i][0] = 0.25*(1 - ksiSurface[i])*(1 - etaSurface[i]);
		NSurface[i][1] = 0.25*(1 + ksiSurface[i])*(1 - etaSurface[i]);
		NSurface[i][2] = 0.25*(1 + ksiSurface[i])*(1 + etaSurface[i]);
		NSurface[i][3] = 0.25*(1 - ksiSurface[i])*(1 + etaSurface[i]);
	}
}


UniversalElement::~UniversalElement(){}


array <array <long double, 4>, 4> &UniversalElement::getN()
{
	return N;
}


array <array <long double, 4>, 8> &UniversalElement::getNSurface()
{
	return NSurface;
}


array <array <long double, 4>, 4> &UniversalElement::getdNdEta()
{
	return dNdEta;
}


array <array <long double, 4>, 4> &UniversalElement::getdNdKsi()
{
	return dNdKsi;
}