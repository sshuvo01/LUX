#pragma once
#include "Core.h"

class Mask : public lux::Volume<double>
{
private:
	lux::Volume<double>* m_Elem;
public:
	Mask(lux::Volume<double>* elem);
	~Mask();

	const double eval(const lux::Vector& x) const;
};

class SpecialMask : public lux::Volume<double>
{
public:
	SpecialMask(lux::Volume<double>* elem);
	~SpecialMask();

	const double eval(const lux::Vector& x) const;
private:
	lux::Volume<double>*	m_Elem;
	double					m_LeftX, m_RightX;
	double					m_UpY, m_DownY;
	double					m_FarZ, m_NearZ;
};

