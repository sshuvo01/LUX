#include "VFNPT.h"
#include "WarpVector.h"
#include <iostream>

VFNPT::VFNPT(lux::Volume<double>* sfElem)
	:m_SFElem(sfElem),
	m_VFScalarGrad(sfElem)
{
}

VFNPT::~VFNPT()
{
}

const lux::Vector VFNPT::eval(const lux::Vector & x) const
{
	lux::Vector grad = m_VFScalarGrad.eval(x);
	double gradMag = grad.magnitude();

	//return m_VFIdentity.eval(x) - 
		//(m_SFElem->eval(x) * grad) / (gradMag * gradMag);
	return x - (m_SFElem->eval(x) * grad) / (gradMag * gradMag);
}


/* iterated transform */
VFNPTn::VFNPTn(lux::Volume<double>* sfElem, int n)
	:m_SFElem(sfElem),
	m_IterationN(n),
	m_NPTn(nullptr)
{
	CalculateTransform();
}

VFNPTn::~VFNPTn()
{
	for (VFBase* vf : m_VFVector)
	{
		if (vf != nullptr)
		{
			delete vf;
			vf = nullptr;
		}
	}

	for (lux::Volume<double>* sf : m_SFVector)
	{
		if (sf != nullptr)
		{
			delete sf;
			sf = nullptr;
		}
	}
}

const lux::Vector VFNPTn::eval(const lux::Vector & x) const
{
	return m_NPTn->eval(x);
}

void VFNPTn::CalculateTransform()
{
	if (m_IterationN <= 0) return;

	VFNPT* npt = new VFNPT(m_SFElem); SavePointerVF(npt);

	WarpVector* vf = new WarpVector(npt, npt); SavePointerVF(vf);
	WarpVector* vf1 = new WarpVector(npt, vf); SavePointerVF(vf1);
	WarpVector* vf2 = new WarpVector(npt, vf1); SavePointerVF(vf2);


	// warp = new warpScalar(npt, npt);

	/*test for 3*/
	/*
	for (int i = 1; i < m_IterationN; i++)
	{
		WarpScalar* warp = new WarpScalar(m_SFElem, npt); SavePointerSF(warp);
		VFNPT* npt = new VFNPT(warp); SavePointerVF(npt);
	}
	*/
	m_NPTn = vf2;
}

void VFNPTn::SavePointerVF(VFBase * p)
{
	m_VFVector.push_back(p);
}

void VFNPTn::SavePointerSF(lux::Volume<double>* p)
{
	m_SFVector.push_back(p);
}

void VFNPTn::DeleteMemory(VFBase * p)
{
	if (p != nullptr)
	{
		delete p;
		p = nullptr;
	}
}
