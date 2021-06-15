#include "SFTranslate.h"


SFTranslate::SFTranslate(lux::Volume<double>* elem, const lux::Vector& dx)
	:m_Elem(elem), m_Dx(dx)
{
}

SFTranslate::~SFTranslate()
{

}

const double SFTranslate::eval(const lux::Vector & x) const
{
	return m_Elem->eval(x-m_Dx);
}
