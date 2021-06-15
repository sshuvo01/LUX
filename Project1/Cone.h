#pragma once
#include "Volume.h"
#include "Vector.h"
#include <cmath>

class Cone : public lux::Volume<double>
{
private:
	double			m_H, m_Theta;
	lux::Vector		m_X0, m_N;
public:
	 Cone(double h, double theta, const lux::Vector& x0, const lux::Vector& n);
	~ Cone();

	const double eval(const lux::Vector& x) const;
};

