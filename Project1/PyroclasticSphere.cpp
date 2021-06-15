#include "PyroclasticSphere.h"

PyroclasticSphere::PyroclasticSphere(double radius, double gamma, 
	const lux::Noise_t& fspnParams, const GridDetail&  gridSet, bool storeingrid)
	: m_Radius(radius),
	m_Gamma(gamma),
	m_Grid(gridSet, -100000.0),
	m_StoreInGrid(storeingrid)
{
	m_FSPN.setParameters(fspnParams);
	
	if(m_StoreInGrid) StoreInGrid();
}

PyroclasticSphere::~PyroclasticSphere() 
{
}

const double PyroclasticSphere::eval(const lux::Vector & x) const
{
	return m_Grid.eval(x);
}

void PyroclasticSphere::StampInAGrid(ScalarGrid * grid, const lux::Vector & sphereTranslate) const
{
	GridDetail setting = grid->GetSetting();
	Sphere sp(m_Radius);
	SFTranslate spT(&sp, sphereTranslate);

	for (int i = 0; i < setting.sizeN.i; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			for (int k = 0; k < setting.sizeN.k; k++)
			{
				lux::Vector x = grid->GridPointToVector({ i, j, k });
				double fx = spT.eval(x);
				double xMag = x.magnitude();
				double gx = fx + 
					std::pow(std::abs(m_FSPN.eval((x/xMag) * m_Radius)), m_Gamma);
				
				double oldGridVal = grid->GetGridData({ i, j, k });
				grid->SetGridData({ i, j, k }, std::max(oldGridVal, gx));
			}
		}
	}
}

void PyroclasticSphere::SetOctaves(double octvs)
{
	lux::Noise_t param;
	m_FSPN.getParameters(param);
	param.octaves = octvs;
	m_FSPN.setParameters(param);
}

void PyroclasticSphere::SetFreq(double freq)
{
	lux::Noise_t param;
	m_FSPN.getParameters(param);
	param.frequency = freq;
	m_FSPN.setParameters(param);
}

void PyroclasticSphere::SetFJump(double fj)
{
	lux::Noise_t param;
	m_FSPN.getParameters(param);
	param.fjump = fj;
	m_FSPN.setParameters(param);
}

void PyroclasticSphere::StoreInGrid()
{
	m_Grid.Clear(0.0);
	GridDetail setting = m_Grid.GetSetting();

	for (int i = 0; i < setting.sizeN.i; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			for (int k = 0; k < setting.sizeN.k; k++)
			{
				lux::Vector x = m_Grid.GridPointToVector({ i, j, k });
				lux::Vector xr = x;
				xr.normalize();
				double second = std::pow(std::abs(m_FSPN.eval(xr * m_Radius)), m_Gamma);
				m_Grid.SetGridData({ i, j, k }, m_Radius - x.magnitude() 
					+ second);
			}
		}
	}
}
