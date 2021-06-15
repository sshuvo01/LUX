#pragma once
#include "VFBase.h"
#include "Volume.h"
#include "Vector.h"

class VFAdd : public VFBase
{
public:
	VFAdd(VFBase* elem1, VFBase* elem2);
	~VFAdd();

	const lux::Vector eval(const lux::Vector& x) const override;
	const lux::Matrix grad(const lux::Vector& x) const override;
private:
	VFBase*		m_Elem1;
	VFBase*		m_Elem2;
};


/*subtraction*/
class VFSubtract : public VFBase
{
public:
	VFSubtract(VFBase* elem1, VFBase* elem2);
	~VFSubtract();

	const lux::Vector eval(const lux::Vector& x) const override;
	const lux::Matrix grad(const lux::Vector& x) const override;
private:
	VFBase*		m_Elem1;
	VFBase*		m_Elem2;
};

/* VF * SF */
class VFMultSF : public VFBase
{
public:
	VFMultSF(VFBase* elem, lux::Volume<double>* sfelem);
	~VFMultSF();

	const lux::Vector eval(const lux::Vector& x) const override;
	const lux::Matrix grad(const lux::Vector& x) const override;
private:
	VFBase*					m_Elem;
	lux::Volume<double>*	m_SFElem;
};

/* VF Cross */
class VFCross : public VFBase
{
public:
	VFCross(VFBase* elem1, VFBase* elem2);
	~VFCross();

	const lux::Vector eval(const lux::Vector& x) const override;
	const lux::Matrix grad(const lux::Vector& x) const override;
private:
	VFBase*		m_Elem1;
	VFBase*		m_Elem2;
};

/* VF dot, result is a scalar field */
class VFDot : public lux::Volume<double>
{
public:
	VFDot(VFBase* elem1, VFBase* elem2);
	~VFDot();
	const double eval(const lux::Vector& x) const override;
	//virtual const volumeDataType eval(const Vector& P) const;
	const lux::Vector grad(const lux::Vector& x) const override;
private:
	VFBase*		m_Elem1;
	VFBase*		m_Elem2;
};

