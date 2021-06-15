#include "Renderer.h"
#include <iostream>

Renderer::Renderer(double deltaS, double kappa, bool useLight)
	:m_DeltaS(deltaS), 
	m_Kappa(kappa), 
	m_UseLight(useLight)
{
}

Renderer::~Renderer()
{
}

const lux::Vector Renderer::RayMarch(const Ray & ray) const
{
	// ray.direction should be a unit vector
	double s = ray.sBegin;
	lux::Vector x = ray.origin + s * ray.direction;
	lux::Vector color = lux::Vector();
	double T = 1.0;
	
	double deltaT;
	double colorMult;
	double invKappa = 1.0 / m_Kappa;
	double kappaDelS = m_Kappa * m_DeltaS;

	while (s < ray.sEnd)
	{
		x += ray.direction * m_DeltaS;
		s += m_DeltaS;

		FieldResult res =  ColorField::eval(x);
		if (res.density == 0.0) { s += m_DeltaS; continue; }

		deltaT = exp(-kappaDelS * res.density);
		colorMult = (1 - deltaT) * T * invKappa;
		
		if (m_UseLight)
		{
			//lux::Vector ct = TotalLight(x);
			res.color = lux::Vector::CompWiseMult(res.color, TotalLight(x));
		}

		color += res.color * colorMult;
		T *= deltaT;
	} // end of while

	return color;
}

void Renderer::AddLight(PointLight * light)
{
	m_Lights.push_back(light);
}

const lux::Vector Renderer::TotalLight(const lux::Vector & x) const
{
	//std::cout << "total light called\n";
	/* c * T */
	lux::Vector sum;
	for (PointLight* pl : m_Lights)
	{
		double Tl = exp(- pl->GetKappa() * pl->DSM(x));
		//double Tl = pl->DSM(x);
		//::cout << "Tl: " << Tl << std::endl;
		//std::cout << "dsm(x): " << pl->DSM(x) << std::endl;
		sum += pl->GetColor() * Tl; 
	}

	return sum;
}
