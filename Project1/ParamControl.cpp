#include "ParamControl.h"
#include <iostream>

ParamControl::ParamControl(double startVal, double endVal, int noOfSteps, int offset)
	:m_StartVal(startVal),
	m_EndVal(endVal),
	m_NofSteps(noOfSteps),
	m_OffsetStep(offset)
{
	m_Increment = (m_EndVal - m_StartVal) / (double)m_NofSteps;
	m_CurrentVal = m_OffsetStep * m_Increment + m_StartVal;

}

ParamControl::~ParamControl()
{
}

const double ParamControl::NextVal() 
{
	if (m_CurrentVal > m_EndVal || m_OffsetStep >= m_NofSteps)
	{
		//std::cout << "This is the end,  current val... " << m_CurrentVal << std::endl;
		//std::cout << "This is the end, offset... " << m_OffsetStep << std::endl;
		return -1.0;
	}
	double retVal = m_CurrentVal;
	m_CurrentVal += m_Increment;
	m_OffsetStep++;
	return retVal;
}

void ParamControl::Reset()
{
	m_OffsetStep = 0;
	m_CurrentVal = m_OffsetStep * m_Increment + m_StartVal;
}
