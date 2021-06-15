#pragma once
#include "Volume.h"
#include "Vector.h"
#include "VFNPT.h"
#include "VFCPT.h"
#include "WarpScalar.h"
#include <algorithm>
#include <cstdlib>
#include "Noise.h"
#include "PerlinNoise.h"
#include "VFCPT.h"
//typedef lux::Volume<double> SFBase;
typedef lux::FractalSum<lux::PerlinNoise> FSPNoise;

class PyroDisplaced : public lux::Volume<double>
{
public:
	PyroDisplaced(lux::Volume<double>* sfElem, 
		lux::Noise* noise, double gamma, int n = 3, bool isSDF = false);
	~PyroDisplaced();

	const double eval(const lux::Vector& x) const;
	void SetNoiseParam(const lux::Noise_t& param);
private:
	bool					m_IsSDF = false;
	int						m_IterationN;
	double					m_Gamma;
	lux::Volume<double>*	m_SFElem;
	//FSPNoise				m_FSPN;
	VFNPTn					m_VFNPTn;
	VFCPT					m_VFCPT;
	WarpScalar				m_WarpNPT;
	WarpScalar				m_WarpCPT;
};
