#include "Box.h"

Box::Box(double r, double q)
	:m_R(r), m_q(q)
{
}

Box::~Box()
{
}

const double Box::eval(const lux::Vector & x) const
{
	double p = 2 * m_q; // m_q > 1, larger m_q, sharper corners, m_q=1 gives a sphere
	return std::pow(m_R, p) - std::pow(x.X(), p) - std::pow(x.Y(), p) - std::pow(x.Z(), p);
}
