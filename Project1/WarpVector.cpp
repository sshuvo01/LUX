#include "WarpVector.h"
#include <iostream>

WarpVector::WarpVector(VFBase* elem1, VFBase* elem2)
	:m_Elem1(elem1), m_Elem2(elem2)
{
}

WarpVector::~WarpVector()
{
}

const lux::Vector WarpVector::eval(const lux::Vector & x) const
{
	//std::cout << "Warp Vector\n";
	return m_Elem1->eval(m_Elem2->eval(x));
}
