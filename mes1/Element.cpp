#include "Element.h"
#include "Input.h"
#include <iostream>

using namespace std;

Element::Element(unsigned long int id, array <GlobalNode*, 4> &globalNodes, UniversalElement universalElement, Input *input)
{
	{
		this->id = id;
		this->globalNodes = globalNodes;
	}

	array <long double, 4> detJ;

	long double specificHeat = input->getSpecificHeat();
	long double density = input->getDensity();
	long double conductionRatio = input->getConductivity();
	long double convectionRatio = input->getAlfa();
	long double ambientTemperature = input->getAmbientTemperature();

	array <array <long double, 4>, 4> N = universalElement.getN();

	{

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				localH[i][j] = 0;
			}
		}

		array <array <long double, 4>, 4> NdEta = universalElement.getdNdEta();
		array <array <long double, 4>, 4> NdKsi = universalElement.getdNdKsi();

		{
			for (int i = 0; i < 4; i++)
			{
				array<long double, 4> transformationJacobian = { 0, 0, 0, 0 };
				for (int j = 0; j < 4; j++)
				{
					transformationJacobian[0] += globalNodes[j]->getX()*NdEta[i][j];
					transformationJacobian[1] += globalNodes[j]->getY()*NdEta[i][j];
					transformationJacobian[2] += globalNodes[j]->getX()*NdKsi[i][j];
					transformationJacobian[3] += globalNodes[j]->getY()*NdKsi[i][j];
				}

				detJ[i] = transformationJacobian[0] * transformationJacobian[3] - transformationJacobian[1] * transformationJacobian[2];

				array<long double, 4> reversedTransformationJacobian;
				reversedTransformationJacobian[0] = transformationJacobian[3] / detJ[i];
				reversedTransformationJacobian[1] = (-1)*transformationJacobian[1] / detJ[i];
				reversedTransformationJacobian[2] = (-1)*transformationJacobian[2] / detJ[i];
				reversedTransformationJacobian[3] = transformationJacobian[0] / detJ[i];

				array <long double, 4> dNdX;
				for (int j = 0; j < 4; j++)
				{
					dNdX[j] = reversedTransformationJacobian[0] * NdEta[i][j] + reversedTransformationJacobian[1] * NdKsi[i][j];
				}

				array <long double, 4> dNdY;
				for (int j = 0; j < 4; j++)
				{
					dNdY[j] = reversedTransformationJacobian[2] * NdEta[i][j] + reversedTransformationJacobian[3] * NdKsi[i][j];
				}

				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 4; k++)
					{
						localH[j][k] += (dNdX[j] * dNdX[k] + dNdY[j] * dNdY[k]) * conductionRatio * detJ[i];
					}
				}
			}
		}
	}
	
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				localC[i][j] = 0;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					localC[j][k] += N[i][j] * N[i][k] * density * specificHeat * detJ[i];
				}
			}
		}
	}

	array <array <long double, 4>, 8> NSurface = universalElement.getNSurface();

	{
		{
			array <array <long double, 4>, 4> matrixPc1;
			array <array <long double, 4>, 4> matrixPc2;
			int j;

			for (int i = 0; i < 4; i++)
			{
				j = i + 1;
				if (j == 4)
				{
					j = 0;
				}

				bool onBound1 = globalNodes[i]->getOnBound();
				bool onBound2 = globalNodes[j]->getOnBound();

				if (onBound1 != true || onBound2 != true)
				{
					continue;
				}

				long double sideLength = 0;
				sideLength += (globalNodes[i]->getX() - globalNodes[j]->getX()) * (globalNodes[i]->getX() - globalNodes[j]->getX());
				sideLength += (globalNodes[i]->getY() - globalNodes[j]->getY()) * (globalNodes[i]->getY() - globalNodes[j]->getY());
				sideLength = sqrt(sideLength);

				long double localDetJ = sideLength * 0.5;

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						matrixPc1[i][j] = 0;
						matrixPc2[i][j] = 0;
					}
				}

				matrixPc1[i][i] = NSurface[2*i][i] * NSurface[2*i][i];
				matrixPc1[i][j] = NSurface[2*i][i] * NSurface[2*i][j];
				matrixPc1[j][i] = matrixPc1[i][j];
				matrixPc1[j][j] = NSurface[2*i][j] * NSurface[2*i][j];

				matrixPc2[i][i] = matrixPc1[j][j];
				matrixPc2[i][j] = matrixPc1[j][i];
				matrixPc2[j][i] = matrixPc1[i][j];
				matrixPc2[j][j] = matrixPc1[i][i];

				for (int k = 0; k < 4; k++)
				{
					for (int l = 0; l < 4; l++)
					{
						localH[k][l] += (matrixPc1[k][l] + matrixPc2[k][l]) * convectionRatio * localDetJ;
					}
				}
			}
		}

		{
			localP.fill(0);
			int j;

			for (int i = 0; i < 4; i++)
			{
				j = i + 1;
				if (j == 4)
				{
					j = 0;
				}

				bool onBound1 = globalNodes[i]->getOnBound();
				bool onBound2 = globalNodes[j]->getOnBound();

				if (onBound1 != true || onBound2 != true)
				{
					continue;
				}

				long double sideLength = 0;
				sideLength += (globalNodes[i]->getX() - globalNodes[j]->getX()) * (globalNodes[i]->getX() - globalNodes[j]->getX());
				sideLength += (globalNodes[i]->getY() - globalNodes[j]->getY()) * (globalNodes[i]->getY() - globalNodes[j]->getY());
				sideLength = sqrt(sideLength);

				long double localDetJ = sideLength * 0.5;

				for (int l = 0; l < 4; l++)
				{
					localP[i] += NSurface[2 * i][l] * ambientTemperature * convectionRatio * localDetJ;
					localP[j] += NSurface[2 * j][l] * ambientTemperature * convectionRatio * localDetJ;
				}
			}
		}
	}
}


Element::~Element()
{
}


unsigned long int Element::getId()
{
	return id;
}


array <GlobalNode*, 4> &Element::getGlobalNodes()
{
	return globalNodes;
}


array <array <long double, 4>, 4> &Element::getLocalH()
{
	return localH;
}


array <array <long double, 4>, 4> &Element::getLocalC()
{
	return localC;
}


array <long double, 4> &Element::getLocalP()
{
	return localP;
}