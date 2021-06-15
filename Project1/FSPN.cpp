#include "FSPN.h"

FSPN::FSPN(const FSPNParams& params)
	:m_Params(params)
{
}

FSPN::~FSPN()
{
}

const double FSPN::eval(const lux::Vector & x, const lux::Vector & xt) const
{
	lux::Vector xmxt = x - xt;
	double fspn = 0.0;

	for (int n = 0; n < m_Params.N; n++)
	{
		fspn += std::pow(m_Params.r, n) * CalculateOctave(xmxt, n);
	}

	double mult = (1.0 - m_Params.r) / (1.0 - std::pow(m_Params.r, m_Params.N));
	return m_Params.a * mult * fspn;
}

const double FSPN::CalculateOctave(const lux::Vector& xmxt, int n) const
{
	double fjn = std::pow(m_Params.fj, n);
	return m_PN.eval(xmxt * (m_Params.f * fjn));
}
