#pragma once

#include "Volume.h"
#include "Vector.h"
#include <cmath>

class Icosahedron :public lux::Volume<double>
{
private:
	const double	m_PI, m_T;
public:
	Icosahedron();
	~Icosahedron();

	const double eval(const lux::Vector& x) const;
};
