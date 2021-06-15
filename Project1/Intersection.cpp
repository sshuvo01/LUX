#include "Intersection.h"

Intersection::Intersection(lux::Volume<double>* elem1, lux::Volume<double>* elem2)
	:m_Elem1(elem1), m_Elem2(elem2)
{
}

Intersection::~Intersection()
{
}

const double Intersection::eval(const lux::Vector & x) const
{
	return std::min(m_Elem1->eval(x), m_Elem2->eval(x));
}
