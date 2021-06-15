#include "Advection.h"

/*semi lagrangian advection*/
SLAdvection::SLAdvection(lux::Volume<double>* sfElem, VFBase* vfElem, double deltaT)
	:m_SFElem(sfElem), m_VFElem(vfElem), m_DeltaT(deltaT)
{
}

SLAdvection::SLAdvection()
	:m_SFElem(nullptr), m_VFElem(nullptr), m_DeltaT(0.0)
{

}

SLAdvection::~SLAdvection()
{
}

const double SLAdvection::eval(const lux::Vector & x) const
{
	return m_SFElem->eval(x - m_VFElem->eval(x) * m_DeltaT);
}
/*semi lagrangian advection*/
