#pragma once
#include "ScalarGrid.h"
#include "Vector.h"
#include "Volume.h"
#include "Noise.h"
#include "PerlinNoise.h"
#include <cstdlib>
#include <iostream>

struct NoiseStampParams
{
	double fade, pScale;
	lux::Vector pc;
};

class NoiseStamp : public lux::Volume<double>
{
public:
	NoiseStamp(const lux::Noise_t& fspnParams, 
		const NoiseStampParams& nsParams, const GridDetail& gridSet, bool storeInGrid = true);
	~NoiseStamp();

	const double eval(const lux::Vector& x) const;
	void StoreInGrid();
	void StampInAGrid(ScalarGrid* grid, const NoiseStampParams& param);

	void SetOctaves(double octvs);
	void SetFreq(double freq);
	void SetFJump(double fj);
	void SetAplitude(double ampl);

	inline void SetNSParams(const NoiseStampParams& nsParams) { m_Params = nsParams; }
	inline NoiseStampParams GetNSParams() const { return m_Params; }
	inline void SetFSPNParams(const lux::Noise_t& fspnParams) { m_FSPN.setParameters(fspnParams); }
	inline void GetFSPNParams(lux::Noise_t fspnParams) const { m_FSPN.getParameters(fspnParams); }
	inline void SetFade(double fde) { m_Params.fade = fde; }
private:
	ScalarGrid							m_Grid;
	NoiseStampParams					m_Params;
	lux::FractalSum<lux::PerlinNoise>	m_FSPN;
	/*private functioins*/
	const double Falloff(const lux::Vector& x) const;
};

