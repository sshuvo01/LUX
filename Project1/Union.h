#pragma once
#include "Core.h"

class Union : public lux::Volume<double>
{
private:
	lux::Volume<double>* m_Elem1;
	lux::Volume<double>* m_Elem2;
public:
	Union(lux::Volume<double>* elem1, lux::Volume<double>* elem2);
	~Union();
	const double eval(const lux::Vector& x) const;
};

