#pragma once
#include "Volume.h"
#include "Vector.h"

class SteinerPatch:public lux::Volume<double>
{
private:
public:
	SteinerPatch();
	~SteinerPatch();

	const double eval(const lux::Vector& x) const;
};

