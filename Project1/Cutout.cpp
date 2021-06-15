#include "Cutout.h"

Cutout::Cutout(lux::Volume<double>* elem1, lux::Volume<double>* elem2)
	:m_Elem1(elem1), m_Elem2(elem2)
{
}

Cutout::~Cutout()
{
}

const double Cutout::eval(const lux::Vector & x) const
{
	return std::min(m_Elem1->eval(x), -m_Elem2->eval(x));
}
