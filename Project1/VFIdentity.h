#pragma once
#include "VFBase.h"


class VFIdentity : public VFBase
{
public:
	VFIdentity();
	~VFIdentity();

	const lux::Vector eval(const lux::Vector& x) const;
	const lux::Matrix grad(const lux::Vector& x) const;
private:

};
