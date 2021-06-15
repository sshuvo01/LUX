#include "Union.h"

Union::Union(lux::Volume<double>* elem1, lux::Volume<double>* elem2)
	:m_Elem1(elem1), m_Elem2(elem2)
{
}

Union::~Union()
{
}

const double Union::eval(const lux::Vector & x) const
{
	return std::max(m_Elem1->eval(x), m_Elem2->eval(x));
}
