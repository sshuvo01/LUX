#pragma once

#include "Volume.h"
#include "Vector.h"

class Sphere : public lux::Volume<double>
{
private:
	double m_Radius;
public:
	Sphere(double r);
	~Sphere();
	const double eval(const lux::Vector& x) const;
	//const lux::Vector grad(const lux::Vector& P) const;
};

