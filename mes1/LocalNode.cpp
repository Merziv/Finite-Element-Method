#include "LocalNode.h"



LocalNode::LocalNode(long double eta, long double ksi)
{
	this->eta = eta;
	this->ksi = ksi;
}


LocalNode::~LocalNode()
{
}


long double LocalNode::getEta()
{
	return eta;
}


long double LocalNode::getKsi()
{
	return ksi;
}