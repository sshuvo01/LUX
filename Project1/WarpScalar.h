#pragma once
#include "Volume.h"
#include "Vector.h"
#include "VFBase.h"
#include "Noise.h"
#include "PerlinNoise.h"

class WarpScalar : public lux::Volume<double>
{
public:
	WarpScalar(lux::Volume<double>* sfElem, VFBase* vfElem);
	WarpScalar(lux::Noise* fspn, VFBase* vfElem);
	~WarpScalar();

	const double eval(const lux::Vector& x) const;
	void SetNoiseParam(const lux::Noise_t& param);
private:
	lux::Volume<double>*		m_SFElem;
	VFBase*						m_VFElem;
	lux::Noise*					m_FSPN;
	bool						m_IsFSPN = false;
};
