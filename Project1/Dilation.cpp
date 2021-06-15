#include "Dilation.h"

Dilation::Dilation(lux::Volume<double>* elem, double c)
	:m_Elem(elem), m_C(c)
{
}

Dilation::~Dilation()
{
}

const double Dilation::eval(const lux::Vector & x) const
{
	return m_Elem->eval(x) + m_C; 
}
