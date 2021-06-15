#include "ColorField.h"
#include <iostream>

std::vector<FieldDetail> ColorField::s_ColorFields;

ColorField::ColorField(const lux::Vector& color, lux::Volume<double>* denF)
	: m_ThisField{color, denF}
{
	s_ColorFields.push_back(m_ThisField);
}

ColorField::~ColorField()
{
}

const FieldResult ColorField::eval(const lux::Vector & x)
{
	lux::Vector val = lux::Vector(0.0, 0.0, 0.0);
	double density = 0.0;

	for (int i = 0; i < s_ColorFields.size(); i++)
	{
		double dens = s_ColorFields[i].densityField->eval(x);
		val += s_ColorFields[i].constantColor * dens;
		//std::cout << "ya ya eval" << std::endl;
		density += dens;
	}

	return { val, density };
}

void ColorField::Clear()
{
	s_ColorFields.clear();
}

