#include "Sphere.h"

Sphere::Sphere(double r)
	:m_Radius(r)
{
}

Sphere::~Sphere()
{
}

const double Sphere::eval(const lux::Vector & x) const
{
	return m_Radius - x.magnitude();
}

//const lux::Vector Sphere::grad(const lux::Vector & P) const
//{
//	return lux::Vector();
//}
