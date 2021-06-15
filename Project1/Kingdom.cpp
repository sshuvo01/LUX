#include "Kingdom.h"
#include "MaskGrid.h"
#include "Plane.h"
#include "Box.h"
#include "Mask.h"
#include "PyroDisplaced.h"
#include "Intersection.h"
#include "PerlinNoise.h"
#include "Noise.h"
#include "Torus.h"
#include "Cone.h"
#include "Ellipse.h"
#include "Cutout.h"
#include "SFRotate.h"
#include "NoiseStamp.h"
#include "Union.h"

TerrainNoise::TerrainNoise(const lux::Noise_t& fspnParam, const TerrainNoise::Params& param)
	: m_Params(param)
{
	m_FSPN.setParameters(fspnParam);
}

TerrainNoise::~TerrainNoise()
{
}

const float TerrainNoise::eval(const lux::Vector & x) const
{
	//std::cout << "Terrain noise\n";
	return Gamma(m_FSPN.eval(x));
}
const float TerrainNoise::Gamma(float v) const
{
	if (v >= 0.0)
	{
		return m_Params.upAmplitude * std::pow(v, m_Params.upGamma);
	}
	else
	{
		return - m_Params.downAmplitude * std::pow(-v, m_Params.downGamma);
	}
	//return result;
}
/*end of fractal terrain noise*/

Kingdom::Kingdom() 
	: m_UnifDist(0.0, 1000000.0)
{
}

Kingdom::~Kingdom()
{
}

void Kingdom::Smoke()
{
	std::cout << "Smoke\n";
	/*
	for (int i = 0; i < 1000; i++)
	{
		std::cout << GetRandMersenne(2.0, 3.0) << std::endl;
	}
	*/
	//topTranslate(2.0, -3.4, 5.0);
	QuadBezier curve;
	//curve.p0 = lux::Vector(-5.0, 5.0, 0.0);
	//curve.p1 = lux::Vector(0.0, -5.0, 0.0);
	//curve.p2 = lux::Vector(5.0, 0.0, 0.0);

	curve.p0 = lux::Vector(2.0, -3.6, 5.0);
	curve.p1 = lux::Vector(2.5, -4.6, 0.0);
	curve.p2 = lux::Vector(2.6, -5.1, -2.8);
	//curve.p1 = lux::Vector(3.0, -4.5, 5.0);
	//curve.p2 = lux::Vector(4.1, -5.2, 5.0);

	//QuadBezier curve2;
	//curve2.p0 = lux::Vector(2.0, -3.7, 5.0);
	//curve2.p1 = lux::Vector(2.4, )

	GridDetail gridLights;
	//gridLights.LLC = lux::Vector(-10.0, -10.0, -10.0);
	gridLights.LLC = lux::Vector(-8.0, -8.0, -8.0);
	gridLights.sizeN = { 300, 300, 300 };
	gridLights.delta = lux::Vector(0.05334, 0.05334, 0.05334);

	GridDetail fakeGrid;
	fakeGrid.LLC = lux::Vector();
	fakeGrid.sizeN = { 0, 0, 0 };
	fakeGrid.delta = lux::Vector(0.0, 0.0, 0.0);

	lux::Noise_t fspn;
	fspn.amplitude = 4.6;
	fspn.frequency = 2.5;
	fspn.fjump = 1.4;
	fspn.octaves = 3.232;
	fspn.roughness = 0.9;

	/*
	PyroclasticSphere* pyroSp = 
		new PyroclasticSphere(2.4, 0.8, fspn, fakeGrid, false); m_SFPointers.push_back(pyroSp);
	ScalarGrid* smokeGrid = new ScalarGrid(gridLights, -10000); m_GridPointers.push_back(smokeGrid);
	
	smokeGrid->Clear(0.0);
	//pyroSp->StampInAGrid(smokeGrid, lux::Vector());
	
	double radius = 0.6;
	int steps = 3;
	double interval = 1.0 / steps;
	for (double t = interval; t <= 1.0; t += interval)
	{
		//pyroSp->SetRadius(radius * (1 - t + interval));
		pyroSp->SetRadius(radius);
		lux::Vector p = curve.GetPoint(t);
		pyroSp->StampInAGrid(smokeGrid, p);
	}
	
	MaskGrid* smokeDensity = new MaskGrid(smokeGrid); m_SFPointers.push_back(smokeDensity);
	ColorField smokeColor = ColorField(lux::Vector(1.0, 1.0, 1.0), smokeDensity);
	*/

	//NoiseStamp* pyroSp =
		//new NoiseStamp(2.4, 0.8, fspn, fakeGrid, false); m_SFPointers.push_back(pyroSp);
	NoiseStampParams nsParam;
	nsParam.fade = 0.5;
	nsParam.pc = lux::Vector();
	nsParam.pScale = 2.0;

	NoiseStamp* nsSp = new NoiseStamp(fspn, nsParam, fakeGrid, false); // do it
	ScalarGrid* smokeGrid = new ScalarGrid(gridLights, -10000); m_GridPointers.push_back(smokeGrid);

	smokeGrid->Clear(0.0);
	//pyroSp->StampInAGrid(smokeGrid, lux::Vector());

	double radius = 0.5;
	double amplitude = 4.0;
	int steps = 300; // +1
	double d = 1.5;
	double interval = 1.0 / steps;
	nsSp->SetFade(0.9);

	for (double t = 0.0; t <= 1.0; t += interval)
	{
		//pyroSp->SetRadius(radius * (1 - t + interval));
		//pyroSp->SetRadius(radius);
		lux::Vector p = curve.GetPoint(t);
		nsParam.pc = p + lux::Vector(GetRandMersenne(-t * d, t * d), GetRandMersenne(-t * d, t*d), 
			GetRandMersenne(-t * d, t*d) );
		nsParam.pScale = radius; 
		//nsSp->SetAplitude(amplitude); //amplitude -= 0.2;
		//nsSp->SetFreq()
		if (t > 0.4)
		{
			nsSp->SetOctaves(1.8);
			nsSp->SetFade(0.6);
		}
		if (t > 0.8)
		{
			nsSp->SetOctaves(1.1);
			nsSp->SetFade(0.2);
		}
		if(t < 0.4)
		{
			//std::cout << "yea yea yea";
			nsParam.pc = p + lux::Vector(GetRandMersenne(-t * d, t * d), GetRandMersenne(-t * d, t*d),
				GetRandMersenne(-t * d, t*d));
			nsSp->StampInAGrid(smokeGrid, nsParam);
		}
		nsSp->StampInAGrid(smokeGrid, nsParam);

		radius += 0.0003;
	}

	MaskGrid* smokeDensity = new MaskGrid(smokeGrid); m_SFPointers.push_back(smokeDensity);
	ColorField smokeColor = ColorField(lux::Vector(1.0, 1.0, 1.0), smokeDensity);
}

void Kingdom::Terrain()
{
	std::cout << "Terrain\n";
	lux::Noise_t fspn;
	fspn.amplitude = 2.0;
	fspn.frequency = 0.2;
	fspn.fjump = 0.3;
	fspn.octaves = 1.232;
	fspn.roughness = 0.3;

	TerrainNoise::Params terrNoiseParam;
	terrNoiseParam.upAmplitude = 12.0;
	terrNoiseParam.downAmplitude = 0.1;
	terrNoiseParam.upGamma = 2.8;
	terrNoiseParam.downGamma = 0.06;

	m_Noise = new TerrainNoise(fspn, terrNoiseParam);
	/*
	lux::Noise* nois = new TerrainNoise(fspn, terrNoiseParam);
	lux::Noise* noisfspn = new lux::FractalSum<lux::PerlinNoise>();
	nois->eval(lux::Vector());
	noisfspn->eval(lux::Vector());
	std::cin.get();
	*/
	Plane* terrainBase = 
		new Plane(lux::Vector(0.0, -1.0, 0.0), lux::Vector(0.0, 0.0, 0.0)); m_SFPointers.push_back(terrainBase);
	PyroDisplaced* terrainWithNoise = new PyroDisplaced(terrainBase, m_Noise, 0.8, 0, true); 
	m_SFPointers.push_back(terrainWithNoise);
	
	Box* boxClipper = new Box(9.6, 5.0); m_SFPointers.push_back(boxClipper);
	Intersection* terrainClipped = new Intersection(terrainWithNoise, boxClipper); 
	m_SFPointers.push_back(terrainClipped);
	

	// cave
	//lux::Vector caveTranslate(-6.5, -1.1, -1.15);
	lux::Vector caveTranslate(-6.5, -0.4, -0.8);
	Sphere* caveSphere = new Sphere(1.61); m_SFPointers.push_back(caveSphere);
	SFTranslate* caveSphereT = new SFTranslate(caveSphere, caveTranslate); m_SFPointers.push_back(caveSphereT);
	Cutout* terrainWithCave = new Cutout(terrainClipped, caveSphereT); m_SFPointers.push_back(terrainWithCave);

	

	// sphere on top of the caldron? cutout
	lux::Vector topTranslate(2.0, -3.4, 5.0); // 2.0, -3.6, 5.0
	Sphere* spTop = new Sphere(1.0); m_SFPointers.push_back(spTop);
	SFTranslate* spTopT = new SFTranslate(spTop, topTranslate); m_SFPointers.push_back(spTopT);

	//Mask* spTopDens = new Mask(spTopT); m_SFPointers.push_back(spTopDens);
	//ColorField spTopColor = ColorField(lux::Vector(0.0, 1.0, 0.0), spTopDens);
	Cutout* finalTerrain = new Cutout(terrainWithCave, spTopT); m_SFPointers.push_back(finalTerrain);

	Mask* planeMask = new Mask(finalTerrain); m_SFPointers.push_back(planeMask);
	ColorField planeColor = ColorField(lux::Vector(114.0 / 255.0, 84.0 / 255.0, 40.0 / 255.0), planeMask);
}

void Kingdom::LandBridge()
{
	std::cout << "LandBridge\n";
	lux::Vector translate(-1.3, 0.0, -2.2);
	double rotateAngle = 45;

	lux::Noise_t fspn;
	fspn.amplitude = 140.0;
	fspn.frequency = 1.666;
	fspn.fjump = 1.2;
	fspn.octaves = 3.232;
	fspn.roughness = 0.9;

	lux::FractalSum<lux::PerlinNoise>* noise = new lux::FractalSum<lux::PerlinNoise>;
	noise->setParameters(fspn);

	Torus* sfTorus = new Torus(3.0, 0.6, lux::Vector(0.0, 0.0, 1.0)); m_SFPointers.push_back(sfTorus);
	PyroDisplaced* pyroTorus = new PyroDisplaced(sfTorus, noise, 0.8, 3);  m_SFPointers.push_back(pyroTorus);

	Plane* plane =
		new Plane(lux::Vector(0.0, -1.0, 0.0), lux::Vector(0.0, 0.0, 0.0)); m_SFPointers.push_back(plane);
	Cutout* landBridge = new Cutout(pyroTorus, plane); m_SFPointers.push_back(landBridge);
	
	SFRotate* landBridgeR = new SFRotate(landBridge, lux::Vector(0.0, -1.0, 0.0), rotateAngle); m_SFPointers.push_back(landBridgeR);
	SFTranslate* landBridgeT = new SFTranslate(landBridgeR, translate); m_SFPointers.push_back(landBridgeT);

	Mask* landBridgeDens = new Mask(landBridgeT); m_SFPointers.push_back(landBridgeDens);
	ColorField landBridgeCol(lux::Vector(252.0/255.0, 3.0/255.0, 111.0/255.0), landBridgeDens);
}

void Kingdom::Monument()
{
	std::cout << "Monument\n";
	lux::Vector atranslate(5.2, 0.5, -6.8);
	lux::Vector spTranslate(0.0, -2.0, 0.0);
	lux::Vector sp2Translate(0.0, -2.6, 0.0);

	Ellipse* ellipse = new Ellipse(3.1, 0.51, lux::Vector(0.0, -1.0, 0.0)); m_SFPointers.push_back(ellipse);
	//SFTranslate* ellipseT = new SFTranslate(ellipse, translate); m_SFPointers.push_back(ellipseT);

	Sphere* sphere = new Sphere(0.6); m_SFPointers.push_back(sphere);
	SFTranslate* sphereT = new SFTranslate(sphere, spTranslate); m_SFPointers.push_back(sphereT);

	Sphere* sphere2 = new Sphere(0.7); m_SFPointers.push_back(sphere2);
	SFTranslate* sphere2T = new SFTranslate(sphere2, sp2Translate); m_SFPointers.push_back(sphere2T);

	Union* spEllipse = new Union(ellipse, sphereT); m_SFPointers.push_back(spEllipse);
	Union* monument = new Union(spEllipse, sphere2T); m_SFPointers.push_back(monument);

	SFTranslate* monumentT = new SFTranslate(monument, atranslate); m_SFPointers.push_back(monumentT);

	Mask* monumentDens = new Mask(monumentT); m_SFPointers.push_back(monumentDens);
	ColorField monumentCol(lux::Vector(7.0/255.0, 203.0/255.0, 237.0/255.0), monumentDens);
}

double Kingdom::GetRandMersenne(double a, double b) 
{
	//m_UnifDist.min = a;
	//m_UnifDist.max = b;
	//std::uniform_real_distribution<double> distribution;// (a, b);
	double distance = b - a;
	double randNormalized = m_UnifDist(m_RandGenerator) / 1000000.0;
	double randNormalizedB = randNormalized * distance;

	return randNormalizedB + a;
}

