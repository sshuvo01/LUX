#pragma once
#include "VFBase.h"
#include "Volume.h"
#include "Vector.h"
#include "VFIdentity.h"
#include "VFScalarGrad.h"

class VFCPT : public VFBase
{
public:
	VFCPT(lux::Volume<double>* sfElem);
	~VFCPT();

	const lux::Vector eval(const lux::Vector& x) const override;
private:
	lux::Volume<double>*		m_SFElem;
	VFScalarGrad				m_VFScalarGrad;
	VFIdentity					m_VFIdentity;
};

