#include "Cone.h"
#include <iostream>

Cone::Cone(double h, double theta, const lux::Vector& x0, const lux::Vector& n)
	:m_H(h), m_Theta(theta), m_X0(x0), m_N(n)
{
	m_N.normalize();
	// parameter in degree
	const double PI = 3.14159265359;
	m_Theta = (PI / 180.0) * theta;
}

Cone::~Cone()
{
}

const double Cone::eval(const lux::Vector & x) const
{
	if (x == m_X0) return 0.0;
	lux::Vector xx0 = x - m_X0;
	double xx0n = xx0 * m_N;
	if (xx0n > m_H) return m_H - xx0n;
	if (xx0n < 0) return xx0n;
	
	//std::cout << "cone::eval";
	//std::cin.get();

	double cosInv = std::acos(xx0n / xx0.magnitude());
	return m_Theta - cosInv;
}
