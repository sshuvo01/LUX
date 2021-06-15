#include "SteinerPatch.h"

SteinerPatch::SteinerPatch()
{
}

SteinerPatch::~SteinerPatch()
{
}

const double SteinerPatch::eval(const lux::Vector & x) const
{
	double first = x.X()*x.X()*x.Y()*x.Y();
	double second = x.X()*x.X()*x.Z()*x.Z();
	double third = x.Y()*x.Y()*x.Z()*x.Z();
	double fourth = x.X()*x.Y()*x.Z();

	return -(first+second+third-fourth);
}
