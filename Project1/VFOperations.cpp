#include "VFOperations.h"

VFAdd::VFAdd(VFBase* elem1, VFBase* elem2)
	:m_Elem1(elem1), m_Elem2(elem2)
{
}

VFAdd::~VFAdd()
{
}

const lux::Vector VFAdd::eval(const lux::Vector & x) const
{
	return m_Elem1->eval(x) + m_Elem2->eval(x);
}

const lux::Matrix VFAdd::grad(const lux::Vector & x) const
{
	return m_Elem1->grad(x) + m_Elem2->grad(x);
}


/*subtraction*/
VFSubtract::VFSubtract(VFBase* elem1, VFBase* elem2)
	:m_Elem1(elem1), m_Elem2(elem2)
{
}

VFSubtract::~VFSubtract()
{
}

const lux::Vector VFSubtract::eval(const lux::Vector & x) const
{
	return m_Elem1->eval(x) - m_Elem2->eval(x);
}

const lux::Matrix VFSubtract::grad(const lux::Vector & x) const
{
	return m_Elem1->grad(x) - m_Elem1->grad(x);
}

/* VF * SF */
VFMultSF::VFMultSF(VFBase* elem, lux::Volume<double>* sfelem)
	:m_Elem(elem), m_SFElem(sfelem)
{
}

VFMultSF::~VFMultSF()
{
}

const lux::Vector VFMultSF::eval(const lux::Vector & x) const
{
	return m_Elem->eval(x) * m_SFElem->eval(x);
}

const lux::Matrix VFMultSF::grad(const lux::Vector & x) const
{
	return m_Elem->grad(x) * m_SFElem->eval(x) +
		lux::Matrix::OuterProduct(m_Elem->eval(x), m_SFElem->grad(x));
}

/*VF cross*/
VFCross::VFCross(VFBase* elem1, VFBase* elem2)
	:m_Elem1(elem1), m_Elem2(elem2)
{
}

VFCross::~VFCross()
{
}

const lux::Vector VFCross::eval(const lux::Vector & x) const
{

	return m_Elem1->eval(x) ^ m_Elem2->eval(x);
}

const lux::Matrix VFCross::grad(const lux::Vector & x) const
{
	return lux::Matrix(); // have to change this
}

/* VF dot, result is a scalar field */
VFDot::VFDot(VFBase* elem1, VFBase* elem2)
	:m_Elem1(elem1), m_Elem2(elem2)
{
}

VFDot::~VFDot()
{
}

const double VFDot::eval(const lux::Vector & x) const
{
	return m_Elem1->eval(x) * m_Elem2->eval(x);
}

const lux::Vector VFDot::grad(const lux::Vector & x) const
{
	return m_Elem1->grad(x) * m_Elem2->eval(x) + m_Elem2->grad(x) * m_Elem1->eval(x);
}
