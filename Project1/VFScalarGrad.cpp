#include "VFScalarGrad.h"

VFScalarGrad::VFScalarGrad(lux::Volume<double>* elem)
	:m_Elem(elem)
{
	//m_Elem = elem;
}

VFScalarGrad::~VFScalarGrad()
{
}

const lux::Vector VFScalarGrad::eval(const lux::Vector & x) const
{
	return m_Elem->grad(x);
}

const lux::Matrix VFScalarGrad::grad(const lux::Vector & x) const
{
	return lux::Matrix(); // have to change
}
