#include "VFIdentity.h"

VFIdentity::VFIdentity()
{
}

VFIdentity::~VFIdentity()
{
}

const lux::Vector VFIdentity::eval(const lux::Vector & x) const
{
	return x;
}

const lux::Matrix VFIdentity::grad(const lux::Vector & x) const
{
	lux::Matrix mat;
	mat.Set(0, 0, 1.0);
	mat.Set(1, 1, 1.0);
	mat.Set(2, 2, 1.0);

	return mat;
}
