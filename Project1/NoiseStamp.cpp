#include "NoiseStamp.h"
#include <cstdlib>
#include <algorithm>

NoiseStamp::NoiseStamp(const lux::Noise_t& fspnParams, 
	const NoiseStampParams& nsParams, const GridDetail& gridSet, bool storeInGrid)
	: m_Grid(gridSet, -10000.000),
	m_Params(nsParams)
{
	m_FSPN.setParameters(fspnParams);
	if(storeInGrid) StoreInGrid();
}

NoiseStamp::~NoiseStamp()
{
}

const double NoiseStamp::eval(const lux::Vector & x) const
{
	double gridVal = m_Grid.eval(x);
	if (gridVal <= 0.0) return 0.0;
	return gridVal;

	//return gridVal * Falloff(x);
	//return m_Grid.eval(x);
}

void NoiseStamp::StoreInGrid()
{
	m_Grid.Clear(0.0);
	GridDetail setting = m_Grid.GetSetting();

	for (int i = 0; i < setting.sizeN.i; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			for (int k = 0; k < setting.sizeN.k; k++)
			{
				GridPoint thisGridPoint = { i, j, k };
				lux::Vector thisGridVec = m_Grid.GridPointToVector(thisGridPoint);
				lux::Vector distVec = m_Params.pc - thisGridVec;
				if (distVec.magnitude() > m_Params.pScale)
				{
					continue;
				}
				double gridVal = m_Grid.GetGridData(thisGridPoint);
				double noiseVal = m_FSPN.eval(thisGridVec) * Falloff(thisGridVec);
				m_Grid.SetGridData(thisGridPoint, std::max(gridVal, noiseVal));
			}
		}
	}
}

void NoiseStamp::StampInAGrid(ScalarGrid* grid, const NoiseStampParams& param) 
{
	GridDetail setting = grid->GetSetting();
	m_Params = param;
	for (int i = 0; i < setting.sizeN.i; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			for (int k = 0; k < setting.sizeN.k; k++)
			{
				GridPoint thisGridPoint = { i, j, k };
				lux::Vector thisGridVec = grid->GridPointToVector(thisGridPoint);
				lux::Vector distVec = m_Params.pc - thisGridVec;
				if (distVec.magnitude() > m_Params.pScale)
				{
					continue;
				}
				double gridVal = grid->GetGridData(thisGridPoint);
				double noiseVal = m_FSPN.eval(thisGridVec) * Falloff(thisGridVec);
				grid->SetGridData(thisGridPoint, std::max(gridVal, noiseVal));
			}
		}
	}
}

void NoiseStamp::SetOctaves(double octvs)
{
	lux::Noise_t param;
	m_FSPN.getParameters(param);
	param.octaves = octvs;
	m_FSPN.setParameters(param);
}

void NoiseStamp::SetFreq(double freq)
{
	lux::Noise_t param;
	m_FSPN.getParameters(param);
	param.frequency = freq;
	m_FSPN.setParameters(param);
}

void NoiseStamp::SetFJump(double fj)
{
	lux::Noise_t param;
	m_FSPN.getParameters(param);
	param.fjump = fj;
	m_FSPN.setParameters(param);
}

void NoiseStamp::SetAplitude(double ampl)
{
	lux::Noise_t param;
	m_FSPN.getParameters(param);
	param.amplitude = ampl;
	m_FSPN.setParameters(param);
}

const double NoiseStamp::Falloff(const lux::Vector & x) const
{
	lux::Vector xmpc = x - m_Params.pc;
	double ratio = xmpc.magnitude() / m_Params.pScale;
	if (ratio < m_Params.fade)
	{
		return 1.0;
	}
	else if (ratio >= 1.0)
	{
		return 0.0;
	}

	return (1.0 - ratio) / (1.0 - m_Params.fade);
}
