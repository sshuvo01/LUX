#pragma once
#include "Core.h"

class Intersection : public lux::Volume<double>
{
private:
	lux::Volume<double>* m_Elem1;
	lux::Volume<double>* m_Elem2;
public:
	Intersection(lux::Volume<double>* elem1, lux::Volume<double>* elem2);
	~Intersection();

	const double eval(const lux::Vector& x) const;
};

