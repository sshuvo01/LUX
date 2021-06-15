#pragma once
#include "Core.h"

class SFTranslate : public lux::Volume<double>
{
private:
	lux::Volume<double>*	m_Elem;
	lux::Vector				m_Dx;
public:
	SFTranslate(lux::Volume<double>* elem, const lux::Vector& dx);
	~SFTranslate();

	const double eval(const lux::Vector& x) const;
};
