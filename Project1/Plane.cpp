#include "Plane.h"

Plane::Plane(const lux::Vector& n, const lux::Vector& x0)
	:m_N(n), m_X0(x0)
{
	m_N.normalize();
}

Plane::~Plane()
{
}

const double Plane::eval(const lux::Vector & x) const
{
	lux::Vector xx0 = x - m_X0;
	// below the plane, inside
	return -(m_N*xx0);
}
