#pragma once
#include "Vector.h"

struct GridPoint
{
	int i, j, k;
};

struct GridDetail
{
	GridPoint		sizeN;
	lux::Vector		delta;
	lux::Vector		LLC;
};

class ScalarGrid
{
public:
	ScalarGrid(const GridDetail& detail, double val = 0.0);
	~ScalarGrid();
	ScalarGrid() = delete;
	ScalarGrid(const ScalarGrid&) = delete;
	ScalarGrid(const ScalarGrid&&) = delete;
	ScalarGrid& operator=(const ScalarGrid&) = delete;
	ScalarGrid& operator=(const ScalarGrid&&) = delete;

	const double eval(const lux::Vector& x) const;
	const double GetGridData(const GridPoint& point) const;
	void SetGridData(const GridPoint& point, double value);
	void IncreaseGridData(const GridPoint& point, double delta);
	const lux::Vector GridPointToVector(const GridPoint& point) const;
	const GridPoint VectorToGridPoint(const lux::Vector& vec) const;
	void Clear(double val);
	/*-------------------*/
	inline GridDetail GetSetting() const { return m_Setting; }
	inline const double GetBoundaryValue() const { return m_BoundaryValue; }
	void Debug() const;
private:
	double*			m_Data;
	GridDetail		m_Setting;
	double			m_BoundaryValue;
	/*private functions*/
	bool InsideGrid(const GridPoint& point) const;
	double Interpolate(const GridPoint& point, const lux::Vector& w) const;
};