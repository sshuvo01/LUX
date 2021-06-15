#include "Avatar.h"
#define SP(x) m_Memories.push_back(x) 
#include "Vector.h"
#include "Sphere.h"
#include "Cone.h"
#include "SFTranslate.h"
#include "Union.h"
#include "Cutout.h"
#include "ColorField.h"
#include "Mask.h"
#include "Plane.h"
#include "Ellipse.h"
#include "SFRotate.h"
#include "Icosahedron.h"
#include "SFScale.h"
#include "Shell.h"
#include "Torus.h"
#include "Box.h"
#include "SteinerPatch.h"
#include "Intersection.h"

Avatar::Avatar()
	:m_MainScale(1.0), m_Head(nullptr)
{
	m_EyeColor = lux::Vector(253, 0, 0)/255.0;
	m_HeadColor = lux::Vector(229, 229, 229)/255.0;
	m_NoseColor = m_EyeColor;//lux::Vector(212, 12, 76)/255.0;
	m_CrownColor = lux::Vector(0.83137, 0.68627, 0.21568);
	m_LegColor = lux::Vector(12, 200, 12) / 255.0;
	m_BodyColor = lux::Vector(143, 121, 212) / 255.0;
	m_HandColor = lux::Vector(232, 16, 207)/255.0;
	//const lux::Vector torusColor(245.0 / 255.0, 71.0 / 255.0, 195.0 / 255.0);
	
}

Avatar::~Avatar()
{
	for (int i = 0; i < m_Memories.size(); i++)
	{
		FreeMemeroy(m_Memories[i]);
	}
}

lux::Volume<double>* Avatar::Eyes()
{
	Sphere* leftEye = new Sphere(1.2); SP(leftEye);
	//Sphere* rightEye = new Sphere(2.0); SP(rightEye);
	SFTranslate* leftEyeT = new SFTranslate(leftEye, lux::Vector(1.50, -0.50, 2.50)); SP(leftEyeT);
	SFTranslate* rightEyeT = new SFTranslate(leftEye, lux::Vector(-1.50, -0.50, 2.50)); SP(rightEyeT);

	Union* eyes = new Union(leftEyeT, rightEyeT); SP(eyes);

	return eyes;
}

lux::Volume<double>* Avatar::Head()
{
	Sphere* mainHead = new Sphere(3.215); SP(mainHead);
	
	return mainHead;
}


lux::Volume<double>* Avatar::Nose()
{
	Cone* cone = new Cone(2.5, 15.0, lux::Vector(0.0, 0.0, -2.0), lux::Vector(0.0, 0.0, 1.0)); SP(cone);
	Plane* noseSlice = new Plane(lux::Vector(0.0, -1.0, 0.0), lux::Vector()); SP(noseSlice);
	//Cutout* nose = new Cutout(cone, noseSlice); SP(nose);

	lux::Vector noseTranslate(0.0, 1.34, 2.8);
	SFScale* noseS = new SFScale(cone, 1.435); SP(noseS);
	SFRotate* noseSR = new SFRotate(noseS, lux::Vector(-1.0, 0.0, 0.0), 180.0); SP(noseSR);
	SFTranslate* noseSRT = new SFTranslate(noseSR, noseTranslate); SP(noseSRT);

	//Mask* noseDensity = new Mask(noseSRT); SP(noseDensity);
	//ColorField nosColor(m_NoseColor, noseDensity);

	return noseSRT;
}

lux::Volume<double>* Avatar::Ears()
{
	const double earRotateAngle = 35.0;
	const double earDistance = 5.0;
	const lux::Vector rotationAxis = lux::Vector(0.0, 0.0, -1.0);

	Ellipse* ellipse = new Ellipse(3.0, 2, lux::Vector(0.0, -1.0, 0.0)); SP(ellipse);
	Plane* earSlice = new Plane(lux::Vector(0.0, 0.0, 1.0), lux::Vector()); SP(earSlice);
	Cutout* leftEar = new Cutout(ellipse, earSlice); SP(leftEar); 
	
	SFRotate* leftEarR = new SFRotate(leftEar, rotationAxis, earRotateAngle); SP(leftEarR);
	SFTranslate* leftEarRT = new SFTranslate(leftEarR, lux::Vector(-earDistance / 2.0, 0.0, 0.0)); SP(leftEarRT);
	SFRotate* rightEarR = new SFRotate(leftEar, rotationAxis, -earRotateAngle); SP(rightEarR);
	SFTranslate* rightEarRT = new SFTranslate(rightEarR, lux::Vector(earDistance / 2.0, 0.0, 0.0)); SP(rightEarRT);
	Union* bothEars = new Union(leftEarRT, rightEarRT); SP(bothEars);
	
	lux::Vector bothEarsTranslate(0.0, -1.0, 0.0);
	SFTranslate* bothEarsT = new SFTranslate(bothEars, bothEarsTranslate); SP(bothEarsT);

	//Mask* earDensity = new Mask(bothEarsT); SP(earDensity);

	//ColorField earColor = ColorField(m_HeadColor, earDensity);
	return bothEarsT;
}

lux::Volume<double>* Avatar::Crown()
{
	//const lux::Vector crownColor(0.83137, 0.68627, 0.21568);
	//const lux::Vector torusColor(245.0/255.0, 71.0/255.0, 195.0/255.0);

	Icosahedron* ico = new Icosahedron(); SP(ico);
	Plane* slicer = new Plane(lux::Vector(0.0, -1.0, 0.0), lux::Vector()); SP(slicer);
	Cutout* crown = new Cutout(ico, slicer); SP(crown);
	Shell* crownThick = new Shell(crown, 2.0); SP(crownThick);

	lux::Vector crownTranslate = lux::Vector(0.0, -4.3, 0.0);
	SFScale* crownS = new SFScale(crownThick, 0.5); SP(crownS);
	SFTranslate* crownST = new SFTranslate(crownS, crownTranslate); SP(crownST);


	return crownST;
}

lux::Volume<double>* Avatar::LeftHand()
{
	Sphere* sp1 = new Sphere(2.2); SP(sp1);
	Ellipse* arm1 = new Ellipse(4.2, 1.5, lux::Vector(0.0, -1.0, 0.0)); SP(arm1);
	SFTranslate* arm1T = new SFTranslate(arm1, lux::Vector(0.0, 2.3, 0.0)); SP(arm1T);
	Union* upperArm = new Union(sp1, arm1T); SP(upperArm);

	Sphere* sp2 = new Sphere(1.8); SP(sp2);
	SFTranslate* sp2T = new SFTranslate(sp2, lux::Vector(0.0, 6.1, 0.0)); SP(sp2T);
	Union* upperArmSp1 = new Union(upperArm, sp2T); SP(upperArmSp1);

	// tricepes? lower arm
	lux::Vector lowerArmTranslate(0.0, 7.32, 3.5); // 7.5
	Ellipse* lowerArm = new Ellipse(5.0, 1.39, lux::Vector(0.0, 0.0, -1.0)); SP(lowerArm);
	SFTranslate* lowerArmT = new SFTranslate(lowerArm, lowerArmTranslate); SP(lowerArmT);
	Union* upperNlowerArm = new Union(upperArmSp1, lowerArmT); SP(upperNlowerArm);

	// ring
	lux::Vector ringTranslate(0.0, 7.5, 3.5 + 2.1 + 1.5 + 2.1);
	Torus* ring = new Torus(2, 1, lux::Vector(1.0, 0.0, 0.0)); SP(ring);
	SFTranslate* ringT = new SFTranslate(ring, ringTranslate); SP(ringT);

	// final arm
	Union* finalArm = new Union(upperNlowerArm, ringT); SP(finalArm);
	// scale down a bit
	SFScale* finalArmS = new SFScale(finalArm, 0.78); SP(finalArmS);
	
	// density
	//Mask* armDensity = new Mask(finalArm); SP(armDensity);
	//ColorField armColor(lux::Vector(0.5, 0.2, 0.31), armDensity);

	return finalArmS;
}

lux::Volume<double>* Avatar::LeftLeg()
{
	Sphere* sp1 = new Sphere(8.0); SP(sp1);
	SFTranslate* sp1T = new SFTranslate(sp1, lux::Vector(-6.9, 0.0, 0.0)); SP(sp1T);
	Sphere* sp2 = new Sphere(8.0); SP(sp2);
	SFTranslate* sp2T = new SFTranslate(sp2, lux::Vector(6.9, 0.0, 0.0)); SP(sp2T);

	// left leg
	Intersection* leftLeg = new Intersection(sp1T, sp2T); SP(leftLeg);
	SFScale* leftLegS = new SFScale(leftLeg, 1.5); SP(leftLegS);
	//Mask* legDensity = new Mask(leftLegS); SP(legDensity);
	//ColorField legColor(m_LegColor, legDensity);

	return leftLeg;
}

lux::Volume<double>* Avatar::Torso()
{
	Box* upperTorso = new Box(3.6, 5.0); SP(upperTorso);
	Sphere* lowerTorso = new Sphere(5.2); SP(lowerTorso);
	SFTranslate* lowerTorsoT = new SFTranslate(lowerTorso, lux::Vector(0.0, 6.0, 0.0)); SP(lowerTorsoT);
	Union* torso = new Union(upperTorso, lowerTorsoT); SP(torso);

	SFScale* torsoS = new SFScale(torso, 1.4); SP(torsoS);

	//Mask* torsoDensity = new Mask(torso); SP(torsoDensity);
	//ColorField torsoColor(lux::Vector(0.23, 0.2, 0.31), torsoDensity);

	return torsoS;
}

lux::Volume<double>* Avatar::FinalAvatar()
{
	using volume = lux::Volume<double>;
	using vector = lux::Vector;
	
	volume* head = Head();
	volume* ears = Ears();
	volume* nose = Nose();
	volume* eyes = Eyes();

	Union* headNEars = new Union(head, ears); SP(headNEars);
	Cutout* headNEarsMNose = new Cutout(headNEars, nose); SP(headNEarsMNose);
	Cutout* headNEarsMNoseMEyes = new Cutout(headNEarsMNose, eyes); SP(headNEarsMNoseMEyes);
	
	vector headTranslate(0.0, -3.0, 0.0);
	// head
	SFTranslate* headT = new SFTranslate(headNEarsMNoseMEyes, headTranslate); SP(headT);
	Mask* headDen = new Mask(headT); SP(headDen);
	ColorField headColor(m_HeadColor, headDen);
	
	// nose
	SFTranslate* noseT = new SFTranslate(nose, headTranslate); SP(noseT);
	Mask* noseDensity = new Mask(noseT); SP(noseDensity);
	ColorField noseColor(m_NoseColor, noseDensity);

	// crown
	volume* crown = Crown();
	SFTranslate* crownT = new SFTranslate(crown, headTranslate); SP(crownT);
	Mask* crownDensity = new Mask(crownT); SP(crownDensity);
	ColorField cr(m_CrownColor, crownDensity);
	
	// eyes
	SFTranslate* eyesT = new SFTranslate(eyes, headTranslate); SP(eyesT);
	Mask* eyesDensity = new Mask(eyesT); SP(eyesDensity);
	ColorField eyesCol(m_EyeColor, eyesDensity);


	// arms
	// left arm
	vector leftHandTranslate(-5.0, 3.2, 0.0);
	vector zAxis(0.0, 0.0, 1.0);
	vector xAxis(1.0, 0.0, 0.0);

	double leftHandRotZ = 22.30;
	double leftHandRotX = 20.02;


	volume* leftHand = LeftHand();
	SFRotate* leftHandRz = new SFRotate(leftHand, zAxis, leftHandRotZ); SP(leftHandRz);
	SFRotate* leftHandRzRx = new SFRotate(leftHandRz, xAxis, leftHandRotX); SP(leftHandRzRx);
	SFTranslate* leftHandRT = new SFTranslate(leftHandRzRx, leftHandTranslate); SP(leftHandRT);

	Mask* leftHandDen = new Mask(leftHandRT); SP(leftHandDen);
	ColorField leftHandCol(m_HandColor, leftHandDen);
	
	// right arm
	vector rightHandTranslate(5.0, 3.2, 0.0);
	double rightHandRotZ = -25.30;
	double rightHandRotX = -15.02;
	
	SFRotate* rightHandRz = new SFRotate(leftHand, zAxis, rightHandRotZ); SP(rightHandRz);
	SFRotate* rightHandRzRx = new SFRotate(rightHandRz, xAxis, rightHandRotX); SP(rightHandRzRx);
	SFTranslate* rightHandT = new SFTranslate(rightHandRzRx, rightHandTranslate); SP(rightHandT);
	
	Mask* rightHandDen = new Mask(rightHandT); SP(rightHandDen);
	ColorField rightHandCol(m_HandColor, rightHandDen);

	// left leg
	double legScale = 1.36;
	vector leftLegTranslate(-4.78, 17.55, 0.0);
	vector rightLegTranslate(4.78, 17.55, 0.0);

	volume* leftLeg = LeftLeg();
	SFScale* legS = new SFScale(leftLeg, legScale); SP(legS);
	SFTranslate* leftLegST = new SFTranslate(legS, leftLegTranslate); SP(leftLegST);

	Mask* leftLegDens = new Mask(leftLegST); SP(leftLegDens);
	ColorField leftLegCol(m_LegColor, leftLegDens);
	// right leg
	SFTranslate* rightLegST = new SFTranslate(legS, rightLegTranslate); SP(rightLegST);

	Mask* rightLegDens = new Mask(rightLegST); SP(rightLegDens);
	ColorField rightLegCol(m_LegColor, rightLegDens);

	// torso
	vector torsoTranslate(0.0, 4.612, 0.0);
	volume* torso = Torso();
	SFTranslate* torsoT = new SFTranslate(torso, torsoTranslate); SP(torsoT);
	
	Cutout* torsoMLarm = new Cutout(torsoT, leftHandRT); SP(torsoMLarm);
	Cutout* torsoMLarmRarm = new Cutout(torsoMLarm, rightHandT); SP(torsoMLarmRarm);
	Cutout* torsoMLarmRarmMHead = new Cutout(torsoMLarmRarm, headT); SP(torsoMLarmRarmMHead);

	Mask* torsoDensity = new Mask(torsoMLarmRarmMHead); SP(torsoDensity);
	ColorField torsoColor(m_BodyColor, torsoDensity);

	// steiner patch on the chest
	lux::Vector patchTranslate(0.0, 3.312, 5.7679); // 6.7679
	SteinerPatch* patch = new SteinerPatch(); SP(patch);
	SFScale* patchS = new SFScale(patch, 4.55); SP(patchS);
	SFTranslate* patchST = new SFTranslate(patchS, patchTranslate); SP(patchST);
	Mask* patchDensity = new Mask(patchST); SP(patchDensity);
	ColorField patchColor(m_CrownColor, patchDensity);

	return nullptr;
}

void Avatar::SavePointer(lux::Volume<double>* p)
{
	m_Memories.push_back(p);
}

void Avatar::FreeMemeroy(lux::Volume<double>* p)
{
	if (p != nullptr)
	{
		delete p;
		p = nullptr;
	}
}
