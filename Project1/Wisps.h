#pragma once
#include <iostream>
#include <cstdlib>
#include "ScalarGrid.h"
#include "FSPN.h"
#include "Volume.h"
#include <random>
#include "Noise.h"
typedef long long int llint;

struct WispsParams
{
	double clump, delta, density;
};

class Wisps : public lux::Volume<double>
{
public:
	Wisps(const lux::Noise_t* nParams, const WispsParams& wParams,
		const GridDetail& gridSet, const lux::Vector& Pc, double pScale, llint n);
	~Wisps();

	const double eval(const lux::Vector& x) const;
	void CalculateWisps();
	/*for some parameters*/
	void SetOctaves(double octvs);
	void SetFreq1(double freq);
	void SetFreq2(double freq);
	void SetFJump(double fj);
	inline void SetClump(double clmp) { m_WParams.clump = clmp; }
private:
	//FSPN			m_FSPN1, m_FSPN2;
	lux::FractalSum<lux::PerlinNoise>	m_FSPN1, m_FSPN2;
	WispsParams		m_WParams;
	ScalarGrid		m_Grid;
	lux::Vector		m_Pc;
	double			m_PScale;
	llint			m_NumberOfDots;
	lux::Vector		m_LLC, m_L;//		m_GridSetting;
	GridPoint		m_N;
	std::mt19937	m_RandGenerator;
	std::uniform_real_distribution<double> m_UnifDist;
	/*private functions*/
	double GetRand() const;
	double GetRandMersenne();
	void BakeDot(const lux::Vector& q);
};

