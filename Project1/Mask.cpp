#include "Mask.h"
#include <iostream>

Mask::Mask(lux::Volume<double>* elem)
	:m_Elem(elem)
{
}

Mask::~Mask()
{

}

const double Mask::eval(const lux::Vector & x) const
{
	double val = m_Elem->eval(x);
	if (val > 0.0)
	{
		//std::cout << "Mask greater than 1\n";
		return 1.0;
	}
	return 0.0;
}
// another class
SpecialMask::SpecialMask(lux::Volume<double>* elem)
	:m_Elem(elem)
{
	m_LeftX = -7.9;
	m_RightX = 7.9;
	
	m_UpY = -7.9;
	m_DownY = 7.9;

	m_FarZ = -2.5;
	m_NearZ = 2.5;
}

SpecialMask::~SpecialMask()
{
}

const double SpecialMask::eval(const lux::Vector & x) const
{
	if (x.X() < m_LeftX || x.X() > m_RightX) return 0.0;
	//if (x.Y() < m_UpY || x.Y() > m_DownY) return 0.0;
	//if (x.Z() < m_FarZ || x.Z() > m_NearZ) return 0.0;

	double val = m_Elem->eval(x);
	if (val > 0.0)
	{
		return 1.0;
	}
	return 0.0;
}
