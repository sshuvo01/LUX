#include "VelocityFields.h"

/*---------PerlinVelocity-----------*/
PerlinVelocity::PerlinVelocity(const lux::Noise_t& fspnParam, const lux::Vector& d)
	:m_D(d)
{
	lux::Noise_t param = fspnParam;
	m_FSPNX.setParameters(fspnParam);
	
	param.translate = d;
	m_FSPNY.setParameters(param);
	
	param.translate = -d;
	m_FSPNZ.setParameters(param);
}

PerlinVelocity::~PerlinVelocity()
{
}

const lux::Vector PerlinVelocity::eval(const lux::Vector & x) const
{
	return lux::Vector(m_FSPNX.eval(x), m_FSPNY.eval(x), m_FSPNZ.eval(x));
}
/*---------PerlinVelocity-----------*/