#include "PointLight.h"
#include "Vector.h"
#include "ColorField.h"
#include <iostream>
#include <thread>

PointLight::PointLight(const lux::Vector& color, const lux::Vector& position, 
	const GridDetail& setting, double deltaS, double kappa, bool calculateDSM)
	:m_Color(color),
	m_Position(position),
	m_DSMGrid(setting, 0.0),
	m_DeltaS(deltaS),
	m_Kappa(kappa), 
	m_MaxThreadN(setting.sizeN.k)
{
	if(calculateDSM) CalculateDSM();
	//m_Threads.resize(m_MaxThreadN);
	//m_ThreadPool.resize(m_MaxThreadN);
}

PointLight::~PointLight()
{
}

void PointLight::CalculateDSM()
{
	GridDetail setting = m_DSMGrid.GetSetting();
	m_DSMGrid.Clear(0.0);

	for (int i = 0; i < setting.sizeN.i; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			for (int k = 0; k < setting.sizeN.k; k++)
			{
				lux::Vector x = m_DSMGrid.GridPointToVector({ i, j, k });
				lux::Vector nl = (m_Position - x);
				double sMax = nl.magnitude();
				nl.normalize();
				double dsmVal = 0.0;

				for (double s = 0; s < sMax; s += m_DeltaS)
				{
					FieldResult res = ColorField::eval(x + (nl * s));
					if (res.density <= 0.0) break;
					dsmVal += m_DeltaS * res.density;
				}
				//std::cout << "dsmVal: " << dsmVal << std::endl;
				// exp(- pl->GetKappa() * pl->DSM(x))
				//double expKDSM = exp(-m_Kappa * dsmVal);
				//m_DSMGrid.SetGridData({i, j, k}, dsmVal);
				m_DSMGrid.SetGridData({ i, j, k }, dsmVal);
			}
		}
	}
}

void PointLight::CalculateDSMmt()
{
	GridDetail setting = m_DSMGrid.GetSetting();
	m_DSMGrid.Clear(0.0);

	int nrCall = setting.sizeN.k;
	int d = (nrCall / m_MaxThreadN);
	int fraction = nrCall - d * m_MaxThreadN;

	for (int i = 0; i < setting.sizeN.i; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			int k = 0;
			while (k < nrCall)
			{
				int upLimit = k + m_MaxThreadN;
				if (upLimit > nrCall) upLimit = k + fraction;

				int vecIt = 0;
				for (; k < upLimit; k++)
				{
					GridPoint gp = { i, j, k };
					m_ThreadPool[vecIt++] = std::thread(&PointLight::WorkerThread, this, gp);
				}
				
				for (int jj = 0; jj < vecIt; jj++)
				{
					m_ThreadPool[jj].join();
				}
			}
		}
	}
}
static int saveCount = 0;

void PointLight::CalculateDSMmt2()
{
	m_DSMGrid.Clear(0.0);
	// divide calculation into 3 threads
	/*
	std::thread threadI(&PointLight::WorkerThread2, this, PointLight::ThreadVar::I);
	std::thread threadJ(&PointLight::WorkerThread2, this, PointLight::ThreadVar::J);
	std::thread threadK(&PointLight::WorkerThread2, this, PointLight::ThreadVar::K);

	threadI.join();
	threadJ.join();
	threadK.join();
	*/

	const int nOfThreads = 2;
	m_ThreadPool.resize(nOfThreads);
	int iBegin, iEnd;
	int iSize = m_DSMGrid.GetSetting().sizeN.i;
	int iInterval = std::ceil((double)iSize / (double)nOfThreads);

	iBegin = 0;
	for (int it = 0; it < nOfThreads; it++)
	{
		if (iBegin >= iSize) break;
		iEnd = iBegin + iInterval;
		if (iEnd > iSize) iEnd = iSize;
		std::cout << "iBegin: " << iBegin << ", iEnd: " << iEnd << std::endl;
		m_ThreadPool[it] = std::thread(&PointLight::WorkerThread2, this, iBegin, iEnd);
		iBegin = iEnd;
	}


	for (int it = 0; it < nOfThreads; it++)
	{
		m_ThreadPool[it].join();
	}

	std::cout << "Saved " << saveCount << std::endl;
}

void PointLight::WorkerThread(const GridPoint& point)
{
	m_GridMutex.lock();
	lux::Vector x = m_DSMGrid.GridPointToVector(point);
	m_GridMutex.unlock();

	lux::Vector nl = (m_Position - x);
	double sMax = nl.magnitude();
	nl.normalize();
	double dsmVal = 0.0;

	for (double s = 0; s < sMax; s += m_DeltaS)
	{
		m_ColorMutex.lock();
		FieldResult res = ColorField::eval(x + (nl * s));
		m_ColorMutex.unlock();

		if (res.density <= 0.0) break;
		dsmVal += m_DeltaS * res.density;
	}

	m_GridMutex.lock(); 
	m_DSMGrid.SetGridData(point, dsmVal);
	m_GridMutex.unlock();
}

void PointLight::WorkerThread2(int iBegin, int iEnd)
{
	m_GridMutex.lock();
	GridDetail setting = m_DSMGrid.GetSetting();
	m_GridMutex.unlock();

	/*
	GridPoint vLimit = setting.sizeN;
	switch (tv)
	{
	case PointLight::ThreadVar::I:
		vLimit.i = 1;
		break;
	case PointLight::ThreadVar::J:
		vLimit.j = 1;
		break;
	case PointLight::ThreadVar::K:
		vLimit.k = 1;
		break;
	default:
		break;
	}
	
	std::cout << "limit: " << vLimit.i << std::endl;
	std::cout << vLimit.j << std::endl;
	std::cout << vLimit.k << std::endl << std::endl;
	*/
	m_PositionMutex.lock();
	lux::Vector lightPos = m_Position;
	double deltaS = m_DeltaS;
	m_PositionMutex.unlock();

	for (int i = iBegin; i < iEnd; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			for (int k = 0; k < setting.sizeN.k; k++)
			{
				//std::cout << "{i, j, k}: " << i << ", " << j << ", " << k << std::endl;
				GridPoint point = { i, j, k };
				m_GridMutex.lock();
				lux::Vector x = m_DSMGrid.GridPointToVector(point);
				m_GridMutex.unlock();

				lux::Vector nl = (lightPos - x);
				double sMax = nl.magnitude();
				nl.normalize();
				double dsmVal = 0.0;
				//std::cout << "smax:"<< sMax << std::endl;

				for (double s = 0; s < sMax; s += deltaS)
				{
					m_ColorMutex.lock();
					FieldResult res = ColorField::eval(x + (nl * s));
					m_ColorMutex.unlock();

					if (res.density <= 0.0) break;
					dsmVal += deltaS * res.density;
				}
				
				m_GridMutex.lock();
				//std::cout << "saving... ";
				//std::cout << "{i, j, k}: " << point.i << ", " << point.j << ", " << point.k << std::endl;
				//if (point.i == 9 && point.j == 0 && point.k ==9) std::cin.get();
				m_DSMGrid.SetGridData(point, dsmVal);
				//saveCount++;
				m_GridMutex.unlock();
			}
		}
	}
}
