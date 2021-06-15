#pragma once
#include "Volume.h"
#include <vector>

class Avatar
{
public:
	Avatar();
	~Avatar();
	
	Avatar(const Avatar& copy) = delete;
	Avatar(const Avatar&& copy) = delete;
	Avatar& operator=(const Avatar& rhs) = delete;
	Avatar& operator=(const Avatar&& rhs) = delete;

	lux::Volume<double>* Head();
	lux::Volume<double>* Eyes();
	lux::Volume<double>* Nose();
	lux::Volume<double>* Ears();
	lux::Volume<double>* Crown();
	lux::Volume<double>* LeftHand();
	lux::Volume<double>* LeftLeg();
	lux::Volume<double>* Torso();
	lux::Volume<double>* FinalAvatar();
private:
	double					m_MainScale;
	lux::Vector				m_HeadColor;
	lux::Vector				m_EyeColor;
	lux::Vector				m_CrownColor;
	lux::Vector				m_NoseColor;
	lux::Vector				m_BodyColor;
	lux::Vector				m_HandColor;
	lux::Vector				m_LegColor;

	lux::Volume<double>*	m_Head;

	std::vector<lux::Volume<double>* > m_Memories;
	/*some private functions*/
	void SavePointer(lux::Volume<double>* p);
	void FreeMemeroy(lux::Volume<double>* p);
};
