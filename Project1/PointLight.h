#pragma once
#include "ScalarGrid.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

class PointLight
{
public:
	enum class ThreadVar
	{
		I,
		J,
		K
	};
	
	PointLight(const lux::Vector& color, const lux::Vector& position, 
		const GridDetail& setting, double deltaS, double kappa, bool calculateDSM = true);
	~PointLight();

	inline const double DSM(const lux::Vector& x) { return m_DSMGrid.eval(x); }
	inline const lux::Vector GetColor() const { return m_Color; }
	inline const lux::Vector GetPosition() const { return m_Position; }
	inline const double GetKappa() const { return m_Kappa; }
	void CalculateDSM();
	void CalculateDSMmt();
	void CalculateDSMmt2();
	inline void DebugDSM() const { m_DSMGrid.Debug(); };
private:
	lux::Vector					m_Color;
	lux::Vector					m_Position;
	ScalarGrid					m_DSMGrid;
	double						m_DeltaS;
	double						m_Kappa;
	/*multithreading data*/
	std::mutex					m_ColorMutex, m_GridMutex, m_PositionMutex;
	std::vector<std::thread>	m_ThreadPool;
	const int					m_MaxThreadN;
	/*private functions*/
	void WorkerThread(const GridPoint& point);
	void WorkerThread2(int iBegin, int iEnd);
};
