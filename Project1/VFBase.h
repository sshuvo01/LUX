#pragma once
#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

class VFBase
{
public:
	VFBase() {};
	virtual ~VFBase() {};

	virtual const lux::Vector eval(const lux::Vector& x) const = 0;
	virtual const lux::Matrix grad(const lux::Vector& x) const { /*needs change*/  return lux::Matrix(); }
private:

};
