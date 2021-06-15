#pragma once
#include "Core.h"

class Shell : public lux::Volume<double>
{
private:
	lux::Volume<double>*	m_Elem;
	double					m_H;
public:
	Shell(lux::Volume<double>* elem, double h);
	~Shell();

	const double eval(const lux::Vector& x) const;
};

