#include "Shell.h"

Shell::Shell(lux::Volume<double>* elem, double h)
	:m_Elem(elem), m_H(h)
{
}

Shell::~Shell()
{
}

const double Shell::eval(const lux::Vector & x) const
{
	double halfH = m_H * 0.5;
	double fx = m_Elem->eval(x);

	double first = fx + halfH;
	double second = fx - halfH;

	return std::min(first, -second);
}

