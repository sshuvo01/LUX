#pragma once
#include "Vector.h"
#include "Volume.h"
#include "PyroclasticSphere.h"
#include "PyroDisplaced.h"
#include "ColorField.h"
#include <random>

struct QuadBezier
{
	lux::Vector p0, p1, p2;
	const lux::Vector GetPoint(double t) const
	{
		double oneMt = 1 - t;
		return oneMt * oneMt * p0 + 2 * oneMt * t * p1 + t * t * p2;
	}
};

/*fractal terrain noise*/
class TerrainNoise : public lux::FractalSum<lux::PerlinNoise>
{
public:
	struct Params
	{
		float upAmplitude, downAmplitude, upGamma, downGamma;
	};
	TerrainNoise(const lux::Noise_t& fspnParam, const TerrainNoise::Params& param);
	~TerrainNoise();
	const float eval(const lux::Vector& x) const override;
private:
	lux::FractalSum<lux::PerlinNoise>		m_FSPN;
	TerrainNoise::Params					m_Params;
	/*private functions*/
	const float Gamma(float v) const;
};
/*fractal terrain noise*/

class Kingdom
{
public:
	Kingdom();
	~Kingdom();
	void Smoke();
	void Terrain();
	void LandBridge();
	void Monument();
private:
	std::vector<lux::Volume<double>*>		m_SFPointers;
	std::vector<VFBase*>					m_VFPointers;
	std::vector<ScalarGrid*>				m_GridPointers;
	lux::Noise*								m_Noise = nullptr;
	std::mt19937	m_RandGenerator;
	std::uniform_real_distribution<double> m_UnifDist;
	/*some private functions*/
	double GetRandMersenne(double a, double b);
};

