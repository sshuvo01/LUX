#include "Wisps.h"

Wisps::Wisps(const lux::Noise_t* nParams, const WispsParams& wParams,
	const GridDetail& gridSet, const lux::Vector& Pc, double pScale, llint n)
	:m_WParams(wParams),
	m_Grid(gridSet, -1000.00),
	m_Pc(Pc),
	m_PScale(pScale),
	m_NumberOfDots(n),
	m_UnifDist(-1.0, 1.0)
{
	//double clump = 1.2; 
	//double pScale = 2.0;
	//lux::Vector Pc(0.0, 0.0, 0.0);
	//double delta = 0.1;
	////
	m_FSPN1.setParameters(nParams[0]);
	m_FSPN2.setParameters(nParams[1]);

	GridDetail setting = m_Grid.GetSetting();
	m_LLC = m_Grid.GetSetting().LLC;
	m_L[0] = (setting.sizeN.i) * setting.delta.X();
	m_L[1] = (setting.sizeN.j) * setting.delta.Y();
	m_L[2] = (setting.sizeN.k) * setting.delta.Z();
	m_N = setting.sizeN;

	//std::cout << m_L.X() << ", " << m_L.Y() << ", " << m_L.Z() << std::endl;
	CalculateWisps();
}

Wisps::~Wisps()
{
}

const double Wisps::eval(const lux::Vector & x) const
{
	//std::cout << "asda\n";
	double val = m_Grid.eval(x);
	if (val > 0.0)
	{
		return val;
	}
	return 0.0;
}

double Wisps::GetRand() const
{
	int r = rand() % 101; // 0 to 100
	double normalized = (double)r / 100.0;
	return normalized * 2 - 1.0; 
}

double Wisps::GetRandMersenne()
{
	return m_UnifDist(m_RandGenerator);
}

void Wisps::CalculateWisps()
{
	m_Grid.Clear(0.0);

	//m_Grid.Debug();

	lux::Vector deltaVec(m_WParams.delta, m_WParams.delta, m_WParams.delta);

	for (llint i = 0; i < m_NumberOfDots; i++)
	{
		lux::Vector X(GetRandMersenne(), GetRandMersenne(), GetRandMersenne());
		lux::Vector X1p = X / X.magnitude();
		double r = std::pow(std::abs(m_FSPN1.eval(X)), m_WParams.clump);
		lux::Vector X2p = r * X1p;
		lux::Vector X3p = (m_PScale * X2p) + m_Pc;  

		double dx = m_FSPN2.eval(X2p);
		double dy = m_FSPN2.eval(X2p + deltaVec);
		double dz = m_FSPN2.eval(X2p - deltaVec);

		BakeDot(X3p + lux::Vector(dx, dy, dz));
	}
	m_Grid.Debug();
}

void Wisps::SetOctaves(double octvs)
{
	lux::Noise_t param;
	m_FSPN1.getParameters(param);
	param.octaves = octvs;
	m_FSPN1.setParameters(param);
}

void Wisps::SetFreq1(double freq)
{
	lux::Noise_t param;
	m_FSPN1.getParameters(param);
	param.frequency = freq;
	m_FSPN1.setParameters(param);
}

void Wisps::SetFreq2(double freq)
{
	lux::Noise_t param;
	m_FSPN2.getParameters(param);
	param.frequency = freq;
	m_FSPN1.setParameters(param);
}

void Wisps::SetFJump(double fj)
{
	lux::Noise_t param;
	m_FSPN1.getParameters(param);
	param.fjump = fj;
	m_FSPN1.setParameters(param);
}

void Wisps::BakeDot(const lux::Vector & q)
{
	lux::Vector r = (q - m_LLC);

	if (r.X() < 0.0 || r.X() > m_L.X()) { std::cout << "outside grid?\n"; return; }
	if (r.Y() < 0.0 || r.Y() > m_L.Y()) { std::cout << "outside grid?\n"; return; }
	if (r.Z() < 0.0 || r.Z() > m_L.Z()) { std::cout << "outside grid?\n"; return; }
	
	lux::Vector rp;
	rp[0] = (r.X() / m_L.X()) * (double)(m_N.i - 1);
	rp[1] = (r.Y() / m_L.Y()) * (double)(m_N.j - 1);
	rp[2] = (r.Z() / m_L.Z()) * (double)(m_N.k - 1);
	/*index coordinate*/
	GridPoint point = { (int)rp.X(), (int)rp.Y(), (int)rp.Z() };
	/*weights*/
	double Wi = rp.X() - (double)point.i;
	double Wj = rp.Y() - (double)point.j;
	double Wk = rp.Z() - (double)point.k;
	/*bake*/
	double oneMWi = 1.0 - Wi;
	double oneMWj = 1.0 - Wj;
	double oneMWk = 1.0 - Wk;

	m_Grid.IncreaseGridData(point, m_WParams.density * oneMWi * oneMWj * oneMWk);
	m_Grid.IncreaseGridData({ point.i + 1, point.j, point.k }, m_WParams.density 
		* Wi * oneMWj * oneMWk);
	m_Grid.IncreaseGridData({ point.i, point.j + 1, point.k }, m_WParams.density
		* oneMWi * Wj * oneMWk);
	m_Grid.IncreaseGridData({ point.i, point.j, point.k + 1 }, m_WParams.density
		* oneMWi * oneMWj * Wk);
	m_Grid.IncreaseGridData({ point.i + 1, point.j + 1, point.k }, m_WParams.density
		* Wi * Wj * oneMWk);
	m_Grid.IncreaseGridData({ point.i + 1, point.j, point.k + 1 }, m_WParams.density
		* Wi * oneMWj * Wk);
	m_Grid.IncreaseGridData({ point.i, point.j + 1, point.k + 1 }, m_WParams.density
		* oneMWi * Wj * Wk);
	m_Grid.IncreaseGridData({ point.i + 1, point.j + 1, point.k + 1 }, m_WParams.density
		* Wi * Wj * Wk);
}
