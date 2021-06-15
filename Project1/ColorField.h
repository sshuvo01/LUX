#pragma once
#include "Core.h"
#include "ScalarGrid.h"
#include <vector>

struct FieldDetail
{
	lux::Vector constantColor;
	lux::Volume<double>* densityField;
};

struct FieldResult
{
	lux::Vector color;
	double density;
};

class ColorField
{
private:
	FieldDetail								m_ThisField;
	static std::vector<FieldDetail>			s_ColorFields;
public:
	ColorField(const lux::Vector& color, lux::Volume<double>* denF = nullptr);
	~ColorField();

	static const FieldResult eval(const lux::Vector& x);
	static void Clear();
};
