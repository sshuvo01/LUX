#include "MaskGrid.h"

MaskGrid::MaskGrid(ScalarGrid* grid)
	:m_Grid(grid)
{
}

MaskGrid::~MaskGrid()
{
}

const double MaskGrid::eval(const lux::Vector & x) const
{
	double val = m_Grid->eval(x);
	if (val > 0.0)
	{
		//return val;
		return 1.0;
	}
	else
	{
		return 0.0;
	}
}
