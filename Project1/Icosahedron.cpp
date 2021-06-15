#include "Icosahedron.h"

Icosahedron::Icosahedron()
	:m_PI(3.1416), m_T(1.61803399)
{
}

Icosahedron::~Icosahedron()
{
}

const double Icosahedron::eval(const lux::Vector & x) const
{
	if (x.magnitude() > 1.8 * m_PI) return -1.8 * m_PI;
	//if(x.magnitude() <= 1.8 * m_PI) 
	return cos(x.X() + m_T * x.Y()) + cos(x.X() - m_T * x.Y()) 
		+ cos(x.Y() + m_T*x.Z()) + cos(x.Y()- m_T*x.Z())
		+ cos(x.Z() - m_T*x.X()) + cos(x.Z() + m_T*x.X()) - 2.0;
}
