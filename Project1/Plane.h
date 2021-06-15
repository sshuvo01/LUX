#pragma once
#include "Volume.h"
#include "Vector.h"

class Plane : public lux::Volume<double>
{
private:
	lux::Vector m_N, m_X0;
public:
	Plane(const lux::Vector& n, const lux::Vector& x0);
	~Plane();

	const double eval(const lux::Vector& x) const;
};

