#include "SFRotate.h"

SFRotate::SFRotate(lux::Volume<double>* elem, const lux::Vector& axis, double angle)
	:m_Elem(elem), m_Axis(axis), m_Angle(angle)
{
	/* calculate the rotation matrix */
	/* angle given in degree */
	const double PI = 3.14159265359;
	double angleRad = (PI / 180.0) * m_Angle;
	double cosT = std::cos(angleRad);
	double sinT = std::sin(angleRad);

	lux::Matrix first = lux::Matrix::Identity() * cosT;
	lux::Matrix second = lux::Matrix::OuterProduct(axis, axis) * (1 - cosT);
	lux::Matrix third = lux::Matrix();

	for (int k = 0; k < 3; k++)
	{
		third += lux::Matrix::PauliMatrix(k) * m_Axis(k) * sinT;
	}

	m_RotationMatrix = first + second + third;	
	m_RotationMatrixInv = m_RotationMatrix.inverse();
}

SFRotate::~SFRotate()
{
}

const double SFRotate::eval(const lux::Vector & x) const
{
	return m_Elem->eval(m_RotationMatrixInv * x);
}
