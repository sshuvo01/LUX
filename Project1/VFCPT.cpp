#include "VFCPT.h"

VFCPT::VFCPT(lux::Volume<double>* sfElem)
	:m_SFElem(sfElem), m_VFScalarGrad(sfElem)
{
}

VFCPT::~VFCPT()
{
}

const lux::Vector VFCPT::eval(const lux::Vector & x) const
{
	return m_VFIdentity.eval(x) - m_SFElem->eval(x) 
		* m_VFScalarGrad.eval(x); //m_SFElem->grad(x);
}
