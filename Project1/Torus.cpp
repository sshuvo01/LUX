#include "Torus.h"

Torus::Torus(double rMajor, double rMinor, const lux::Vector& n)
	:m_RMajor(rMajor), m_RMinor(rMinor), m_N(n)
{
	m_N.normalize();
}

Torus::~Torus()
{
}

const double Torus::eval(const lux::Vector & x) const
{
	double xn = x * m_N;
	lux::Vector xPerp = x - (xn*m_N);
	
	double first = 4 * pow(m_RMajor, 2)*pow(xPerp.magnitude(), 2);
	double second = pow(x.magnitude(), 2) + pow(m_RMajor, 2) - pow(m_RMinor, 2);

	return first - pow(second, 2);
}
