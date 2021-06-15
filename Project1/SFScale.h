#pragma once
#include "Core.h"

class SFScale : public lux::Volume<double>
{
private:
	lux::Volume<double>*	m_Elem;
	double					m_T;
public:
	SFScale(lux::Volume<double>* elem, double t);
	~SFScale();

	const double eval(const lux::Vector& x) const;
};

