#pragma once
#include "VFBase.h"
#include "Volume.h"

class VFScalarGrad : public VFBase
{
public:
	VFScalarGrad(lux::Volume<double>* elem);
	~VFScalarGrad();

	const lux::Vector eval(const lux::Vector& x) const;
	const lux::Matrix grad(const lux::Vector& x) const;
private:
	lux::Volume<double>*	m_Elem;
};

