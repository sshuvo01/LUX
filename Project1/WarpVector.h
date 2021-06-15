#pragma once
#include "VFBase.h"

class WarpVector : public VFBase
{
public:
	WarpVector(VFBase* elem1, VFBase* elem2);
	~WarpVector();

	const lux::Vector eval(const lux::Vector& x) const override;
private:
	VFBase*		m_Elem1;
	VFBase*		m_Elem2;
};
