#pragma once
#include "Core.h"
#include "Matrix.h"
#include <cmath>

class SFRotate : public lux::Volume<double>
{
private:
	lux::Volume<double>*		m_Elem;
	lux::Vector					m_Axis;
	double						m_Angle;
	lux::Matrix					m_RotationMatrix;
	lux::Matrix					m_RotationMatrixInv;
public:
	SFRotate(lux::Volume<double>* elem, const lux::Vector& axis, double angle);
	~SFRotate();

	const double eval(const lux::Vector& x) const;
};

