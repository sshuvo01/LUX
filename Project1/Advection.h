#pragma once
#include "VFBase.h"
#include "Volume.h"
#include "Vector.h"

/*semi lagrangian advection*/
class SLAdvection : public lux::Volume<double>
{
public:
	SLAdvection(lux::Volume<double>* sfElem, VFBase* vfElem, double deltaT);
	SLAdvection();
	~SLAdvection();

	const double eval(const lux::Vector& x) const;
	inline void SetScalarField(lux::Volume<double>* sfElem) { m_SFElem = sfElem; }
	inline void SetVelocityField(VFBase* vfElem) { m_VFElem = vfElem; }
	inline void SetDeltaT(double deltaT) { m_DeltaT = deltaT; }
private:
	lux::Volume<double>*		m_SFElem;
	VFBase*						m_VFElem;
	double						m_DeltaT;
};
/*semi lagrangian advection*/