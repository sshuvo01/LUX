#pragma once
#include "VFBase.h"
#include "Vector.h"
#include "Noise.h"
#include "PerlinNoise.h"

/*---------PerlinVelocity-----------*/
class PerlinVelocity : public VFBase
{
public:
	PerlinVelocity(const lux::Noise_t& fspnParam, const lux::Vector& d);
	~PerlinVelocity();

	const lux::Vector eval(const lux::Vector& x) const override;
private:
	lux::Vector								m_D;
	lux::FractalSum<lux::PerlinNoise>		m_FSPNX, m_FSPNY, m_FSPNZ;
};
/*---------PerlinVelocity-----------*/