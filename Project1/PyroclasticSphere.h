#pragma once
#include "Volume.h"
#include "Vector.h"
#include "FSPN.h"
#include "ScalarGrid.h"
#include "Sphere.h"
#include "Noise.h"
#include "SFTranslate.h"
#include <algorithm>

class PyroclasticSphere : public lux::Volume<double>
{
public: 
	PyroclasticSphere(double radius, double gamma, const lux::Noise_t& fspnParams, 
		const GridDetail&  gridSet, bool storeingrid = true);
	~PyroclasticSphere();

	const double eval(const lux::Vector& x) const;
	void StampInAGrid(ScalarGrid* grid, const lux::Vector& sphereTranslate) const;

	inline void SetRadius(double r) { m_Radius = r; }
	inline void SetFSPN(const lux::Noise_t& param) { m_FSPN.setParameters(param); }
	inline void SetGamma(double gamma) { m_Gamma = gamma; }
	void SetOctaves(double octvs);
	void SetFreq(double freq);
	void SetFJump(double fj);
	
	void StoreInGrid();
private:
	double								m_Radius;
	double								m_Gamma;
	//FSPN		m_FSPN;
	lux::FractalSum<lux::PerlinNoise>	m_FSPN;
	ScalarGrid							m_Grid;
	lux::Volume<double>*				m_Sphere;
	bool								m_StoreInGrid = true;
	/*private functions*/
};

