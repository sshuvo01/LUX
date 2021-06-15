#pragma once
#include "Core.h"

class Dilation : public lux::Volume<double>
{
private:
	lux::Volume<double>* m_Elem;
	double m_C;
public:
	Dilation(lux::Volume<double>* elem, double c);
	~Dilation();

	const double eval(const lux::Vector& x) const;
};
