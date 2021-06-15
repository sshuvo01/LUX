#pragma once
#include "Volume.h"
#include "Vector.h"

class Ellipse: public lux::Volume<double>
{
private:
	double			m_RMajor, m_RMinor;
	lux::Vector		m_N;
public:
	Ellipse(double rMajor, double rMinor, const lux::Vector& n);
	~Ellipse();

	const double eval(const lux::Vector& x) const;
};

