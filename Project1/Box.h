#pragma once
#include "Volume.h"
#include "Vector.h"
#include <cmath>

class Box : public lux::Volume<double>
{
private:
	double m_R, m_q;
public:
	Box(double r, double q);
	~Box();

	const double eval(const lux::Vector& x) const;
};

