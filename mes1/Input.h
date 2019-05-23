#pragma once
#include <fstream>
using namespace std;

class Input
{
	fstream file;
	static double initial_temperature;
	static double time;
	static double step;
	static double ambient_temperature;
	static double alfa;
	static double H;
	static double L;
	static double nH;
	static double nL;
	static double specific_heat;
	static double conductivity;
	static double density;

public:
	Input();
	~Input();
	double getInitialTemperature();
	double getTime();
	double getStep();
	double getAmbientTemperature();
	double getAlfa();
	double getH();
	double getL();
	double getnH();
	double getnL();
	double getSpecificHeat();
	double getConductivity();
	double getDensity();
};