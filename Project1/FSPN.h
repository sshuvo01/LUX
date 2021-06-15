#pragma once
#include "PerlinNoise.h"
#include "Vector.h"

struct FSPNParams
{
	double a, f, fj, r;
	int	N;
};

class FSPN
{
public:
	FSPN(const FSPNParams& params);
	~FSPN();
	const double eval(const lux::Vector& x, const lux::Vector& xt = lux::Vector(0.0, 0.0, 0.0)) const;
	/*inline functions*/
	inline FSPNParams GetParams() const { return m_Params; }
	inline void SetParams(const FSPNParams& params) { m_Params = params; }
private:
	lux::PerlinNoise	m_PN;
	FSPNParams			m_Params;
	/*private functions*/
	const double CalculateOctave(const lux::Vector& xmxt, int n) const;
};

