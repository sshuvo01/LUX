#include "PyroDisplaced.h"
#include <iostream>

PyroDisplaced::PyroDisplaced(lux::Volume<double>* sfElem,
	lux::Noise* noise, double gamma, int n, bool isSDF)
	:m_SFElem(sfElem),
	m_Gamma(gamma),
	m_IterationN(n),
	m_VFNPTn(sfElem, m_IterationN),
	m_WarpNPT(noise, &m_VFNPTn),
	m_IsSDF(isSDF),
	m_VFCPT(sfElem),
	m_WarpCPT(noise, &m_VFCPT)
{
	//m_FSPN.setParameters(fspnParam);
}

PyroDisplaced::~PyroDisplaced()
{
}

const double PyroDisplaced::eval(const lux::Vector & x) const
{
	double wp;
	if (m_IsSDF)
	{
		wp = m_WarpCPT.eval(x);
		
	}
	else
	{
		wp = m_WarpNPT.eval(x);
	}

	/*
	std::cout << std::pow(std::abs(wp), m_Gamma) << std::endl;
	std::cout << m_Gamma << std::endl;
	*/

	//return m_SFElem->eval(x) + std::pow(std::abs(wp), m_Gamma);
	//return  std::pow(std::abs(wp), m_Gamma);
	//lux::Noise_t aaa;

	//m_FSPN.getParameters(aaa);
	////std::cout << aaa.amplitude << std::endl;
	//std::cin.get();
/*	
	if (m_SFElem->eval(x) > 0.0)
	{
		std::cout << "sf eval: " << m_SFElem->eval(x) << std::endl;
		std::cout << std::pow(std::abs(wp), m_Gamma) << std::endl;
		std::cin.get();
	}
	*/
	//std::cout << wp << std::endl;
	double val = m_SFElem->eval(x);
	//if (val < 0.0)
	//{
		//std::cout << "Warning: negative value! " << val << std::endl;
		//return 0.0;
	//}
	return val + std::pow(std::abs(wp), m_Gamma);
	//return m_SFElem->eval(x) + m_FSPN.eval(x);
}

void PyroDisplaced::SetNoiseParam(const lux::Noise_t& param)
{
	m_WarpNPT.SetNoiseParam(param);
}
