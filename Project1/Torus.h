#pragma once
#include "Volume.h"
#include "Vector.h"
#include <cmath>

class Torus:public lux::Volume<double>
{
private:
	double			m_RMajor, m_RMinor;
	lux::Vector		m_N;
public:
	Torus(double rMajor, double rMinor, const lux::Vector& n);
	~Torus();

	const double eval(const lux::Vector& x) const;
};
