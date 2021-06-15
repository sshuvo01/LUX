#include "WarpScalar.h"


WarpScalar::WarpScalar(lux::Volume<double>* sfElem, VFBase* vfElem)
	:m_SFElem(sfElem), 
	m_VFElem(vfElem), 
	m_IsFSPN(false), 
	m_FSPN(nullptr)
{
}

WarpScalar::WarpScalar(lux::Noise * fspn, VFBase * vfElem)
	:m_SFElem(nullptr), 
	m_VFElem(vfElem),
	m_IsFSPN(true),
	m_FSPN(fspn)
{
}

WarpScalar::~WarpScalar()
{
}

const double WarpScalar::eval(const lux::Vector & x) const
{
	lux::Vector vfX = m_VFElem->eval(x);
	if (m_IsFSPN)
	{ 
		return m_FSPN->eval(vfX);
	}
	else return m_SFElem->eval(vfX);
}

void WarpScalar::SetNoiseParam(const lux::Noise_t & param)
{
	if (m_IsFSPN)
	{
		m_FSPN->setParameters(param);
	}
}
