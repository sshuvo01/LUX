#pragma once

class ParamControl
{
public:
	ParamControl(double startVal, double endVal, int noOfSteps, int offset = 0);
	~ParamControl();

	const double NextVal();

	inline const double GetOffset() const { return m_OffsetStep; }
	void Reset();
private:
	double		m_StartVal, m_EndVal, m_Increment, m_CurrentVal;
	int			m_NofSteps, m_OffsetStep;
};
