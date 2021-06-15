#pragma once
#include "Vector.h"
#include "Volume.h"
#include  "VFBase.h"
#include "VFIdentity.h"
#include "VFScalarGrad.h"
#include "WarpScalar.h"
#include <vector>

/*nearest point transform*/
class VFNPT : public VFBase
{
public:
	VFNPT(lux::Volume<double>* sfElem);
	~VFNPT();

	const lux::Vector eval(const lux::Vector& x) const override;
private:
	lux::Volume<double>*		m_SFElem;
	VFIdentity					m_VFIdentity;
	VFScalarGrad				m_VFScalarGrad;
};

/* iterated nearest point transform */
class VFNPTn : public VFBase
{
public:
	VFNPTn(lux::Volume<double>* sfElem, int n);
	~VFNPTn();

	const lux::Vector eval(const lux::Vector& x) const override;
private:
	int									m_IterationN;
	lux::Volume<double>*				m_SFElem; /*scalar field to do NPT*/
	VFBase*								m_NPTn; /*final NPT*/
	std::vector<VFBase*>				m_VFVector;
	std::vector<lux::Volume<double>*>	m_SFVector;

	/*private functions*/
	void CalculateTransform();
	void SavePointerVF(VFBase* p);
	void SavePointerSF(lux::Volume<double>* p);
	void DeleteMemory(VFBase* p);
};