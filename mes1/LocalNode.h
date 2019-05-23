#pragma once
class LocalNode
{
	double long ksi;
	double long eta;
public:
	LocalNode(long double eta, long double ksi);
	~LocalNode();

	double long getKsi();
	double long getEta();
};