#include "SFScale.h"

SFScale::SFScale(lux::Volume<double>* elem, double t)
	:m_Elem(elem), m_T(t)
{
}

SFScale::~SFScale()
{
}

const double SFScale::eval(const lux::Vector & x) const
{
	return m_Elem->eval(x / m_T);
}
