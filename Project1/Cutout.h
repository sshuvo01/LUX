#pragma once
#include "Core.h"

class Cutout : public lux::Volume<double>
{
private:
	lux::Volume<double>* m_Elem1;
	lux::Volume<double>* m_Elem2;
public:
	Cutout(lux::Volume<double>* elem1, lux::Volume<double>* elem2);
	~Cutout();

	const double eval(const lux::Vector& x) const;
};
