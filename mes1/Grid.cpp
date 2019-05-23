#include "Grid.h"
#include "Input.h"
#include <iostream>
#include <iomanip>
using namespace std;

Grid::Grid()
{
	unsigned long int verticalNodeNumber = input.getnH();
	unsigned long int horizontalNodeNumber = input.getnL();
	long double nodeHeight = input.getH()/ (verticalNodeNumber -1);
	long double nodeWidth = input.getL() / (horizontalNodeNumber -1);

	{
		unsigned int id = 0;
		bool onBound;
		for (unsigned long int i = 0; i < horizontalNodeNumber; i++)
		{
			for (unsigned long int j = 0; j < verticalNodeNumber; j++)
			{
				if (i == 0 || i == horizontalNodeNumber - 1)
				{
					onBound = true;
				}
				else if (j == 0 || j == verticalNodeNumber - 1)
				{
					onBound = true;
				}
				else
				{
					onBound = false;
				}

				nodes.push_back(new GlobalNode(++id, i*nodeWidth, j*nodeHeight, onBound, &input));
			}
		}
	}

	{
		unsigned int id = 0;
		for (unsigned long int i = 0; i < horizontalNodeNumber - 1; i++)
		{
			for (unsigned long int j = 1; j < verticalNodeNumber; j++)
			{
				int nodesId[4];
				nodesId[0] = j + verticalNodeNumber * i;
				nodesId[1] = j + verticalNodeNumber * (i + 1);
				nodesId[2] = j + 1 + verticalNodeNumber * (i + 1);
				nodesId[3] = j + 1 + verticalNodeNumber * i;

				array <GlobalNode*, 4> elementNodes;
				for (int k = 0; k < 4; k++)
				{
					elementNodes[k] = nodes[nodesId[k] - 1];
				}

				elements.push_back(new Element(++id, elementNodes, universalElement, &input));
			}
		}
	}
}


Grid::~Grid()
{
	while (!elements.empty())
	{
		delete elements.back();
		elements.pop_back();
	}

	while (!nodes.empty())
	{
		delete nodes.back();
		nodes.pop_back();
	}
}


long double* gaussSeidel(long double **A, long double *B, unsigned long int n)
{
	int iteration = 10000;
	long double **M,*N, *x, *result;

	{
		N = new long double[n];

		M = new long double*[n];

		x = new long double[n];

		result = new long double[n];

		for (unsigned int i = 0; i < n; i++)
		{
			M[i] = new long double[n];
			x[i] = 0;
		}
	}

	for (unsigned int i = 0; i < n; i++)
	{
		N[i] = 1 / A[i][i];
	}

	for (unsigned int i = 0; i < n; i++)
	{
		for (unsigned int j = 0; j < n; j++)
		{
			if (i == j)
				M[i][j] = 0;
			else
				M[i][j] = -(A[i][j] * N[i]);
		}
	}

	for (int k = 0; k < iteration; k++)
	{
		for (unsigned int i = 0; i < n; i++)
		{
			result[i] = N[i] * B[i];
			for (unsigned int j = 0; j < n; j++)
				result[i] += M[i][j] * x[j];

			x[i] = result[i];
		}
	}

	{
		for (unsigned int i = 0; i < n; i++)
			delete[] M[i];
		delete[] N;
		delete[] M;
		delete[] x;
	}

	return result;
}

void Grid::calculate()
{
	unsigned long int nodeCount = input.getnH()*input.getnL();
	long double **globalH = new long double *[nodeCount];
	long double **globalC = new long double *[nodeCount];
	long double *globalP = new long double [nodeCount];
	long double stepTime = input.getStep();
	long double time = input.getTime();

	cout << "time " << time << " step time " << stepTime << endl;

	bool showT0 = true;
	bool showT1 = true;
	bool showInit = true;

	for (unsigned long int i = 0; i < nodeCount; i++)
	{
		globalH[i] = new long double[nodeCount];
		globalC[i] = new long double[nodeCount];

		for (unsigned long int j = 0; j < nodeCount; j++)
		{
			globalH[i][j] = 0;
			globalC[i][j] = 0;
		}

		globalP[i] = 0;
	}

	{
		for (unsigned long int i = 0; i < elements.size(); i++)
		{
			Element *element = elements[i];
			array <array <long double, 4>, 4> localH = element->getLocalH();
			array <array <long double, 4>, 4> localC = element->getLocalC();
			array <long double, 4> localP = element->getLocalP();

			for (int j = 0; j < 4; j++)
			{
				unsigned long int id1 = element->getGlobalNodes()[j]->getId();

				for (int k = 0; k < 4; k++)
				{
					unsigned long int id2 = element->getGlobalNodes()[k]->getId();
					globalH[id1 - 1][id2 - 1] += localH[j][k];
					globalC[id1 - 1][id2 - 1] += localC[j][k];
				}

				globalP[id1 - 1] += localP[j];
			}
		}
	}

	if (showInit)
	{
		cout << "ITERATION: 0 - START DATA" << endl;
		cout << "[H]" << endl;
		for (int i = 0; i < nodeCount; i++)
		{
			for (int j = 0; j < nodeCount; j++)
			{
				cout << setw(8) << globalH[i][j] << "  ";
			}

			cout << endl;
		}
		cout << endl << endl;


		cout << "[C]" << endl;
		for (int i = 0; i < nodeCount; i++)
		{
			for (int j = 0; j < nodeCount; j++)
			{
				cout << setw(8) << globalC[i][j] << "  ";
			}

			cout << endl;
		}
		cout << endl << endl;
	}

	{
		for (unsigned long int i = 0; i < nodeCount; i++)
		{
			for (unsigned long int j = 0; j < nodeCount; j++)
			{
				globalH[i][j] += globalC[i][j] / stepTime;
			}
		}
	}

	long double *tmpP = new long double[nodeCount];

	for (unsigned long int i = 0; i < nodeCount; i++)
	{
		long double value = 0;

		for (unsigned long int j = 0; j < nodeCount; j++)
		{
			value += globalC[i][j] / stepTime * nodes[j]->getT();
		}

		tmpP[i] = value + globalP[i];
	}

	double iteration;
	double* min;
	double* max;
	double t;

	min = new double[input.getTime()];
	max = new double[input.getTime()];

	if (showInit)
	{
		cout << "[H]+[C]/dT" << endl;
		for (int i = 0; i < nodeCount; i++)
		{
			for (int j = 0; j < nodeCount; j++)
			{
				cout << setw(8) << globalH[i][j] << "  ";
			}

			cout << endl;
		}
		cout << endl << endl;

		cout << "{P}" << endl;
		for (int j = 0; j < nodeCount; j++)
		{
			cout << tmpP[j] << "  ";
		}
		cout << endl << endl;
	}

	for (double z = 0; z < time; z+=stepTime)
	{
		iteration = z / stepTime + 1;
		cout << "ITERATION: " << iteration << endl << endl;

		{
			if (showT0)
			{
				cout << "Temperatures t0" << endl;
				for (int i = 0; i < input.getnH(); i++)
				{
					for (int j = 0; j < input.getnL(); j++)
						cout << nodes[j + (i*input.getnL())]->getT() << "	";

					cout << endl << endl;
				}
			}

			long double *t1 = gaussSeidel(globalH, tmpP, nodeCount);

			for (int i = 0; i < nodeCount; i++)
			{
				nodes[i]->setT(t1[i]);
			}

			min[(int)iteration-1] = max[(int)iteration-1] = t1[0];
			{
				for (unsigned long int i = 0; i < nodeCount; i++)
				{
					long double value = 0;

					for (unsigned long int j = 0; j < nodeCount; j++)
					{
						value += globalC[i][j] / stepTime * nodes[j]->getT();
					}

					tmpP[i] = value + globalP[i];
				}
			}

			if(showT1)
				cout << "Temperatures t1" << endl;
			max[(int)iteration-1] = t1[0];
			for (int i = 0; i < input.getnH(); i++)
			{
				for (int j = 0; j < input.getnL(); j++)
				{
					t = t1[j + (i*(int)input.getnL())];
					if (showT1)
						cout << t << "	";

					if (t < min[(int)iteration-1]) min[(int)iteration-1] = t;
					if (t > max[(int)iteration-1]) max[(int)iteration-1] = t;
				}

				if (showT1)
					cout << endl << endl;
			}
			delete[] t1;
		}

		if (iteration < 2)
		{
			cout << "[H]+[C]/dT" << endl;
			for (int i = 0; i < nodeCount; i++)
			{
				for (int j = 0; j < nodeCount; j++)
				{
					cout << setw(8) << globalH[i][j] << "  ";
				}

				cout << endl;
			}
			cout << endl << endl;

			cout << "{P}" << endl;
			for (int j = 0; j < nodeCount; j++)
			{
				cout << tmpP[j] << "  ";
			}
			cout << endl << endl;
		}

		if (z + stepTime == time)
		{
			cout << setw(5) << "time" << setw(10) << "min temp" << setw(10) << "max temp" << endl;
			for (int i = 0; i < (time / stepTime); i++)
			{
				cout << setw(5) << (i+1)*stepTime << setw(10) << min[i] << setw(10) << max[i] << endl;
			}
		}
	}

	for (unsigned long int i = 0; i < nodeCount; i++)
	{
		delete[] globalH[i];
		delete[] globalC[i];
	}

	delete[] globalH;
	delete[] globalC;
	delete[] globalP;
	delete[] tmpP;
}


void Grid::displayGrid()
{
	cout << "Displaying grid:" << endl;
	for (unsigned long int i = 0; i < elements.size(); i++)
	{
		cout<< elements[i]->getId();
		array <GlobalNode*, 4> nodes = elements[i]->getGlobalNodes();
		cout << " --> ";
		cout << nodes[0]->getId() << ", ";
		cout << nodes[1]->getId() << ", ";
		cout << nodes[2]->getId() << ", ";
		cout << nodes[3]->getId() << endl;
	}
}