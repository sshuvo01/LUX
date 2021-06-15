#pragma once
#include "Core.h"
#include "ColorField.h"
#include <math.h>
#include <vector>
#include "PointLight.h"

struct Ray
{
	lux::Vector		direction;
	lux::Vector		origin;
	double			sBegin;
	double			sEnd;
};

class Renderer
{
public:
	Renderer(double deltaS, double kappa, bool useLight = false);
	~Renderer();

	const lux::Vector RayMarch(const Ray& ray) const;
	void AddLight(PointLight* light);
private:
	double						m_DeltaS;
	double						m_Kappa;
	std::vector<PointLight*>	m_Lights;
	bool						m_UseLight;

	const lux::Vector TotalLight(const lux::Vector& x) const;
};
