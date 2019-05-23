#include "Input.h"
#include <string>

Input::Input()
{
	file.open("Files/input.csv", ios::in);
	if (file.good())
	{
		string line;
		getline(file, line, ':');
		getline(file, line, '\n');
		initial_temperature = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		time = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		step = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		ambient_temperature = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		alfa = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		H = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		L = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		nH = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		nL = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		specific_heat = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		conductivity = stod(line);

		getline(file, line, ':');
		getline(file, line, '\n');
		density = stod(line);


		file.close();
	}
}
Input::~Input() {}

double Input::getInitialTemperature()
{
	return initial_temperature;
}

double Input::getTime()
{
	return time;
}

double Input::getStep()
{
	return step;
}

double Input::getAmbientTemperature()
{
	return ambient_temperature;
}

double Input::getAlfa()
{
	return alfa;
}

double Input::getH()
{
	return H;
}

double Input::getL()
{
	return L;
}

double Input::getnH()
{
	return nH;
}

double Input::getnL()
{
	return nL;
}

double Input::getSpecificHeat()
{
	return specific_heat;
}

double Input::getConductivity()
{
	return conductivity;
}

double Input::getDensity()
{
	return density;
}


double Input::initial_temperature;
double Input::time;
double Input::step;
double Input::ambient_temperature;
double Input::alfa;
double Input::H;
double Input::L;
double Input::nH;
double Input::nL;
double Input::specific_heat;
double Input::conductivity;
double Input::density;