#pragma once
#include "ScalarGrid.h"
#include "Volume.h"
#include "Vector.h"

class MaskGrid : public lux::Volume<double>
{
public:
	MaskGrid(ScalarGrid* grid);
	~MaskGrid();

	const double eval(const lux::Vector& x) const;
private:
	ScalarGrid*			m_Grid;
};