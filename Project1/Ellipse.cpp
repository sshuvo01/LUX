#include "Ellipse.h"


Ellipse::Ellipse(double rMajor, double rMinor, const lux::Vector& n)
	: m_RMajor(rMajor), m_RMinor(rMinor), m_N(n)
{
	m_N.normalize();
}

Ellipse::~Ellipse()
{
}

const double Ellipse::eval(const lux::Vector & x) const
{
	double z = x * m_N;
	lux::Vector xPerp = x - z * m_N;
	double xPerpMag = xPerp.magnitude();

	double second = (z*z) / (m_RMajor*m_RMajor);
	double third = (xPerpMag * xPerpMag) / (m_RMinor*m_RMinor);
	
	return 1 - second - third;
}
