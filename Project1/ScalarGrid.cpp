#include "ScalarGrid.h"
#include <iostream>


ScalarGrid::ScalarGrid(const GridDetail& detail, double val)
	:m_Data(nullptr),
	m_Setting(detail),
	m_BoundaryValue(val)
{
	/*allocate memory*/
	m_Data = new double[m_Setting.sizeN.i * m_Setting.sizeN.j * m_Setting.sizeN.k];
	Clear(val);
}

ScalarGrid::~ScalarGrid()
{
	delete[] m_Data;
}

const double ScalarGrid::eval(const lux::Vector& x) const
{
	//std::cout << x.X() << " " << x.Y() << " " << x.Z() << std::endl;

	double xDelx = (x.X() - m_Setting.LLC.X()) / m_Setting.delta.X();
	double yDely = (x.Y() - m_Setting.LLC.Y()) / m_Setting.delta.Y();
	double zDelz = (x.Z() - m_Setting.LLC.Z()) / m_Setting.delta.Z();

	int i = (int) xDelx;
	int j = (int) yDely;
	int k = (int) zDelz;

	double wi = xDelx - (double)i;
	double wj = yDely - (double)j;
	double wk = zDelz - (double)k;

	/*interpolate*/
	return Interpolate({i, j, k}, {wi, wj, wk});
}

const double ScalarGrid::GetGridData(const GridPoint& point) const
{
	if (!InsideGrid(point)) return m_BoundaryValue;
	return m_Data[point.i + m_Setting.sizeN.i * (point.j + m_Setting.sizeN.k * point.k)];
}

void ScalarGrid::SetGridData(const GridPoint& point, double value)
{
	if (!InsideGrid(point)) return;
	m_Data[point.i + m_Setting.sizeN.i * (point.j + m_Setting.sizeN.k * point.k)] = value;
}

void ScalarGrid::IncreaseGridData(const GridPoint & point, double delta)
{
	if (!InsideGrid(point)) return;
	m_Data[point.i + m_Setting.sizeN.i * (point.j + m_Setting.sizeN.k * point.k)] += delta;
}

const lux::Vector ScalarGrid::GridPointToVector(const GridPoint & point) const
{
	lux::Vector vec(m_Setting.delta.X() * point.i, 
		m_Setting.delta.Y() * point.j, m_Setting.delta.Z() * point.k);

	return lux::Vector(m_Setting.LLC + vec);
}

const GridPoint ScalarGrid::VectorToGridPoint(const lux::Vector & vec) const
{
	GridPoint gp;
	gp.i = std::floor((vec.X() - m_Setting.LLC.X()) / m_Setting.delta.X());
	gp.j = std::floor((vec.Y() - m_Setting.LLC.Y()) / m_Setting.delta.Y());
	gp.k = std::floor((vec.Z() - m_Setting.LLC.Z()) / m_Setting.delta.Z());

	return gp;
}

void ScalarGrid::Clear(double val)
{
	if (val == 0.0)
	{
		memset(m_Data, 0, sizeof(double) * m_Setting.sizeN.i * m_Setting.sizeN.j 
			* m_Setting.sizeN.k);
		return;
	}

	int size = m_Setting.sizeN.i * m_Setting.sizeN.j * m_Setting.sizeN.k;
	for (int i = 0; i < size; i++)
	{
		//std::cout << "clearing with:" << val << std::endl;
		m_Data[i] = val;
	}
}

void ScalarGrid::Debug() const
{
	for (int i = 0; i < m_Setting.sizeN.i; i++)
	{
		for (int j = 0; j < m_Setting.sizeN.j; j++)
		{
			for (int k = 0; k < m_Setting.sizeN.k; k++)
			{
				//std::cout << "{ " << i << ", " << j << ", " << k << " }: " 
					//<< GetGridData({ i, j, k }) << std::endl;

				if (GetGridData({ i, j, k }) < 0.0)
				{
					std::cout << "{ " << i << ", " << j << ", " << k << " }: " 
					<< GetGridData({ i, j, k }) << std::endl;
				}
			}
		}
	}
	std::cout << std::endl;
}

bool ScalarGrid::InsideGrid(const GridPoint& point) const
{
	if (point.i < 0 || point.i >= m_Setting.sizeN.i) return false;
	if (point.j < 0 || point.j >= m_Setting.sizeN.j) return false;
	if (point.k < 0 || point.k >= m_Setting.sizeN.k) return false;

	return true;
}


double ScalarGrid::Interpolate(const GridPoint & point, const lux::Vector & w) const
{

	//std::cout << "interpolate\n";
	//std::cout << point.i << " " << point.j << " " << point.k << std::endl;
	//std::cout << w.X() << " " << w.Y() << " " << w.Z() << std::endl;
	//std::cout << "Inside the grid\n";

	return 
		GetGridData(point) * (1 - w.X()) * (1 - w.Y()) * (1 - w.Z())
		+
		GetGridData({ point.i + 1, point.j, point.k }) * w.X() * (1 - w.Y()) * (1 - w.Z())
		+
		GetGridData({ point.i, point.j + 1, point.k }) * (1 - w.X()) * w.Y() * (1 - w.Z())
		+
		GetGridData({ point.i, point.j, point.k + 1 }) * (1 - w.X()) * (1 - w.Y()) * w.Z()
		+
		GetGridData({ point.i + 1, point.j + 1, point.k }) * w.X() * w.Y() * (1 - w.Z())
		+
		GetGridData({ point.i + 1, point.j, point.k + 1 }) * w.X() * (1 - w.Y()) * w.Z()
		+
		GetGridData({ point.i, point.j + 1, point.k + 1 }) * (1 - w.X()) * w.Y() * w.Z()
		+
		GetGridData({ point.i + 1, point.j + 1, point.k + 1 }) * w.X() * w.Y() * w.Z();
}
