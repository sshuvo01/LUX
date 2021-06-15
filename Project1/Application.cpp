//#define NOMINMAX
//#define _CRT_SECURE_NO_WARNINGS
#include <limits>
#include <algorithm>
//#define TINYEXR_IMPLEMENTATION
//#include "tinyexr.h"
//#define TINYEXR_IMPLEMENTATION

#include <iostream>
#include "Vector.h"
#include "Volume.h"
#include "Sphere.h"
#include "Ellipse.h"
#include "Union.h"
#include "Mask.h"
#include "ColorField.h"
#include "Image.h"
#include "Cone.h"
#include "SFTranslate.h"
#include "Cutout.h"
#include "Torus.h"
#include "SteinerPatch.h"
#include "Icosahedron.h"
#include "Matrix.h"
#include "SFRotate.h"
#include "Box.h"
#include "Avatar.h"
#include "ScalarGrid.h"
#include "LevelSet.h"
#include "PointLight.h"
#include "ModelLoader.h"
#include "SFScale.h"
#include "PerlinNoise.h"
#include "Wisps.h"
#include "PyroclasticSphere.h"
#include "Noise.h"
#include "NoiseStamp.h"
#include "Plane.h"
#include "VFScalarGrad.h"
#include "WarpScalar.h"
#include "PyroDisplaced.h"
#include "MaskGrid.h"
#include "Kingdom.h"
#include <random>
#include <memory>
#include "ParamControl.h"
#include "Advection.h"
#include "VelocityFields.h"
#define timeNow std::chrono::high_resolution_clock::now()

double constDouble()
{
	double flew;
	flew = 124.23;
	return flew;
}

class I
{
public:
	I(int i) : value(i) {}
	void set(int i) { value = i; }
	const I operator+(const I& rhs) { return I(value + rhs.value); }
	I& operator=(const I& rhs) { value = rhs.value; return *this; }
	I() = delete;
	
	int value;
private:
};

static void PrintVector(const std::string& msg, const lux::Vector& vec)
{
	std::cout << msg << ": ";
	std::cout << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;
}

static double DegToRad(double angleDeg)
{
	const double PI = 3.14159265359;
	return (PI / 180.0) * angleDeg;
}

bool pointInTriangle(const lux::Vector& query_point,
	const lux::Vector& triangle_vertex_0,
	const lux::Vector& triangle_vertex_1,
	const lux::Vector& triangle_vertex_2)
{
	// u=P2−P1
	lux::Vector u = triangle_vertex_1 - triangle_vertex_0;
	// v=P3−P1
	lux::Vector v = triangle_vertex_2 - triangle_vertex_0;
	// n=u×v
	lux::Vector n = u^v;
	// w=P−P1
	lux::Vector w = query_point - triangle_vertex_0;
	// Barycentric coordinates of the projection P′of P onto T:
	// γ=[(u×w)⋅n]/n²
	double gamma = ((u^w)*n)/(n*n);
	// β=[(w×v)⋅n]/n²
	float beta = ((w^v)*n) / (n*n);
	float alpha = 1 - gamma - beta;
	// The point P′ lies inside T if:
	return ((0 <= alpha) && (alpha <= 1) &&
		(0 <= beta) && (beta <= 1) &&
		(0 <= gamma) && (gamma <= 1));
}


SLAdvection* GetAdvectedArray(lux::Volume<double>* scalarField, VFBase* velocityField, int n)
{
	/*
	lux::Noise_t fspnParam;
	fspnParam.amplitude = 1.2;
	fspnParam.frequency = 0.666;
	fspnParam.fjump = 1.2;
	fspnParam.octaves = 3.2;
	fspnParam.roughness = 0.6;
	PerlinVelocity velocityField(fspnParam, lux::Vector(0.4, 0.2, 0.8));
	*/
	double deltaT = 0.04;
	//int numberOfSteps = 60;
	// 
	SLAdvection* advection = new SLAdvection[n];
	advection[0].SetDeltaT(deltaT);
	advection[0].SetScalarField(scalarField);
	advection[0].SetVelocityField(velocityField);

	for (int i = 1; i < n; i++)
	{
		advection[i].SetDeltaT(deltaT);
		advection[i].SetVelocityField(velocityField);
		//
		advection[i].SetScalarField(&advection[i - 1]);
	}

	return advection;
}

int mainturnadv()
{
	std::cout << "Bunny turntable advection!\n";
	GridPoint size = { 300, 300, 300 }; //{ 160, 160, 160 }; //{ 80, 80, 80 };
	lux::Vector delta = { 0.0133, 0.0133, 0.0133 }; //{0.11, 0.11, 0.11}; //{ .21, .21, .21 };
	lux::Vector llc(-2.0, -2.0, -2.0);
	GridDetail setting = { size, delta, llc };

	GridPoint dsmSize = { 250, 250, 250 }; // cover 16
	lux::Vector dsmDelta = { 0.064, 0.064, 0.064 };
	lux::Vector dsmLLC(-8.0, -8.0, -8.0);
	GridDetail dsmSetting = { dsmSize, dsmDelta, dsmLLC };


	int nrOfImages = 120;
	int beginFrom = 0;
	std::cout << "Image number to begin from (0-" << nrOfImages - 1 << "): ";
	std::cin >> beginFrom;

	std::string modelPath = "models/cleanbunny.obj";
	LevelSet boxLevelSet(modelPath, setting);

	double xFrom1 = -0.425891, xTo1 = 0.085681;
	double yFrom1 = 0.338662, yTo1 = 0.768145;
	double zFrom1 = -0.297956, zTo1 = 0.104317;

	lux::Vector adjust(0.1, 0.1, 0.1);
	//boxLevelSet.ClearSubGrid(lux::Vector(xFrom, yFrom, zFrom), lux::Vector(xTo, yTo, zTo) + adjust, 0.0);

	double xFrom2 = -0.626607, xTo2 = -0.439698;
	double yFrom2 = 0.308326, yTo2 = 0.707123;
	double zFrom2 = -0.616827, zTo2 = -0.014553;
	//boxLevelSet.ClearSubGrid(lux::Vector(xFrom, yFrom, zFrom), lux::Vector(xTo, yTo, zTo) + adjust, 0.0);

	// advection on ears
	lux::Noise_t fspnParam;
	fspnParam.amplitude = 1.2;
	fspnParam.frequency = 1.666;
	fspnParam.fjump = 1.2;
	fspnParam.octaves = 3.2;
	fspnParam.roughness = 0.6;
	PerlinVelocity velocityField(fspnParam, lux::Vector(0.4, 0.2, 0.8));
	/*ear1*/
	GridPoint ear1Size = { 150, 150, 150 }; // cover 16
	lux::Vector ear1Delta = { 0.0133, 0.0133, 0.0133 };
	lux::Vector ear1LLC(-1.0, -1.0, -1.0);
	GridDetail ear1Setting = { ear1Size, ear1Delta, ear1LLC };

	ScalarGrid ear1Grid(ear1Setting, -920000);
	ear1Grid.Clear(0.0);
	boxLevelSet.StampSubGridInGrid(lux::Vector(xFrom1, yFrom1, zFrom1), 
		lux::Vector(xTo1, yTo1, zTo1), &ear1Grid);
	MaskGrid ear1Mask(&ear1Grid);

	SFScale ear1Scale = SFScale(&ear1Mask, 9.2);
	SFRotate ear1SR = SFRotate(&ear1Scale, lux::Vector(1.0, 0.0, 0.0), 180);
	SLAdvection ear1Advected = SLAdvection(&ear1SR, &velocityField, 0.51);
	ColorField ear1Color(lux::Vector(1.0, 1.0, 1.0), &ear1Advected);
	/*ear2*/
	ScalarGrid ear2Grid(ear1Setting, -920000);
	ear2Grid.Clear(0.0);
	boxLevelSet.StampSubGridInGrid(lux::Vector(xFrom2, yFrom2, zFrom2), 
		lux::Vector(xTo2, yTo2, zTo2), &ear2Grid);
	MaskGrid ear2Mask(&ear2Grid);
	
	SFScale ear2Scale = SFScale(&ear2Mask, 9.2);
	SFRotate ear2SR = SFRotate(&ear2Scale, lux::Vector(1.0, 0.0, 0.0), 180);
	SLAdvection ear2Advected = SLAdvection(&ear2SR, &velocityField, 0.5);
	ColorField ear2color(lux::Vector(1.0, 1.0, 1.0), &ear2Advected);
	/*--------------------------------*/
	boxLevelSet.ClearSubGrid(lux::Vector(xFrom1, yFrom1, zFrom1), 
		lux::Vector(xTo1, yTo1, zTo1) + adjust, 0.0);
	boxLevelSet.ClearSubGrid(lux::Vector(xFrom2, yFrom2, zFrom2), 
		lux::Vector(xTo2, yTo2, zTo2) + adjust, 0.0);
	
	SFScale boxScaled = SFScale(&boxLevelSet, 9.2);
	SFRotate boxSR(&boxScaled, lux::Vector(1.0, 0.0, 0.0), 180.0);
	Mask boxDensity(&boxSR);
	ColorField spColor(lux::Vector(1.0, 1.0, 1.0), &boxDensity);

	std::cout << "Levelset loaded..." << std::endl;
	lux::Vector light1Pos(19.30, -5.0, 5.0);
	lux::Vector light2Pos(-19.30, -5.0, 5.0);
	lux::Vector light3Pos(0.0, -5.0, -19.30);
	lux::Vector light1Color(245.0 / 255.0, 66.0 / 255.0, 191.0 / 255.0);
	lux::Vector light2Color(66.0 / 255.0, 84.0 / 255.0, 245.0 / 255.0);
	lux::Vector light3Color(130.0 / 255.0, 240.0 / 255.0, 62.0 / 255.0);
	/////
	//Sphere sp(6.0);
	//Mask spDen(&sp);
	//ColorField spColor(lux::Vector(1.0, 1.0, 1.0), &spDen);
	// deltaS, Kappa
	double lightDeltaS = 0.1;
	double lightKappa = 0.83;
	PointLight light1(light1Color, light1Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 1\n";
	PointLight light2(light2Color, light2Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 2...\n";
	PointLight light3(light3Color, light3Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 3...\n";

	Renderer rend = Renderer(0.02, 0.7, true);
	rend.AddLight(&light1);
	rend.AddLight(&light2);
	rend.AddLight(&light3);
	//
	CameraSetup camset;
	camset.position = lux::Vector(10.0, 0.0, 0.0);
	camset.viewDirection = lux::Vector(0.0, 5.0, 0.0) - camset.position;
	camset.nearPlane = 14.0;
	camset.farPlane = 60.598; // 60.5
	camset.fov = 100.0; // was 110
	Image img(camset, 16 * 120, 9 * 120);


	/*camera turntable*/

	double degreeInterval = 360.0 / (double)nrOfImages; // 
	double radius = 25.0;
	double currentAngle = 0.0;
	//lux::Vector lookAt = lux::Vector(0.0, 4.98, 0.0);
	lux::Vector lookAt = lux::Vector(0.0, 0.00, 0.0);
	std::string imgName = "bunny_ear_advection";
	std::string folderName = "img5/bunnyear";



	for (int i = beginFrom; i < nrOfImages; i++)
	{
		std::cout << "Generating image#" << i << "..." << std::endl;
		currentAngle = (i * degreeInterval); // mult i, final

		double z = radius * std::cos(DegToRad(currentAngle));
		double x = radius * std::sin(DegToRad(currentAngle));
		camset.position = lux::Vector(x, 0.0, z);
		camset.viewDirection = lookAt - camset.position;
		img.SetupCamera(camset);

		std::string filename = imgName + std::to_string(i);
		filename = folderName + "/" + filename + ".exr";
		//PrintVector("camera position", camset.position);
		auto startTime = timeNow;
		img.CaptureImage(&rend);
		img.WriteImage(filename);
		auto endTime = timeNow;
		std::chrono::duration<float> duration = endTime - startTime;
		std::cout << "Elapsed time: " << duration.count() << " seconds\n\n";
	}

	std::cout << "Done!" << std::endl;



	std::cin.get();
	return 0;
}

void StampInGrid(ScalarGrid* gridToStamp, lux::Volume<double>* sf)
{
	GridDetail setting = gridToStamp->GetSetting();
	
	for (int i = 0; i < setting.sizeN.i; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			for (int k = 0; k < setting.sizeN.k; k++)
			{
				lux::Vector x = gridToStamp->GridPointToVector({ i, j, k });
				gridToStamp->SetGridData({ i, j, k }, sf->eval(x));
			}
		}
	}
}

int main()
{
	std::cout << "Bunny Advection\n";
	std::cout << "Enter image number to begin with (0 - 59): ";
	int imageNum;
	std::cin >> imageNum;

	GridPoint size = { 400, 400, 400 }; //{ 160, 160, 160 }; //{ 80, 80, 80 };
	lux::Vector delta = { 0.01, 0.01, 0.01 }; //{0.11, 0.11, 0.11}; //{ .21, .21, .21 };
	lux::Vector llc(-2.0, -2.0, -2.0);
	GridDetail setting = { size, delta, llc };

	//GridPoint dsmSize = { 250, 250, 250 }; // cover 16
	//lux::Vector dsmDelta = { 0.064, 0.064, 0.064 };
	//lux::Vector dsmLLC(-8.0, -8.0, -8.0);
	//GridDetail dsmSetting = { dsmSize, dsmDelta, dsmLLC };

	std::string modelPath = "models/cleanbunny.obj";
	LevelSet bunnyLevelSet(modelPath, setting);
	SFScale bunnyScaled = SFScale(&bunnyLevelSet, 9.2);

	SFRotate bunnySR(&bunnyScaled, lux::Vector(1.0, 0.0, 0.0), 180.0);
	SFRotate bunnySR2(&bunnySR, lux::Vector(0.0, -1.0, 0.0), 180.0);
	/*pyro displacement*/
	lux::Noise_t bunnyNoiseParam;
	bunnyNoiseParam.amplitude = 0.0826; // 0.06
	bunnyNoiseParam.frequency = 0.666; // 0.666
	bunnyNoiseParam.fjump = 1.2; // 1.2
	bunnyNoiseParam.octaves = 2.2; // 1.2
	bunnyNoiseParam.roughness = 0.5; // 0.5

	lux::FractalSum<lux::PerlinNoise> bunnyNoise;
	bunnyNoise.setParameters(bunnyNoiseParam);
	PyroDisplaced bunnyDisplaced = PyroDisplaced(&bunnySR2, &bunnyNoise, 0.9);
	/*------------------------*/
	SpecialMask bunnyDensity(&bunnyDisplaced);
	GridPoint size2 = { 500, 500, 500 }; 
	lux::Vector delta2 = { 0.032, 0.032, 0.032 }; 
	lux::Vector llc2(-8.0, -8.0, -8.0);
	GridDetail setting2 = { size2, delta2, llc2 };
	ScalarGrid bunnyGrid(setting2, -900000);
	bunnyGrid.Clear(0.0);
	StampInGrid(&bunnyGrid, &bunnyDensity);
	MaskGrid bunnyGridDensity(&bunnyGrid);
	//ColorField spColor(lux::Vector(1.0, 1.0, 1.0), &bunnyDensity);

	Sphere sp = Sphere(4.0);
	Mask spDensity(&sp);
	//ColorField spColor(lux::Vector(1.0, 1.0, 1.0), &spDensity);
	/*velocity field*/
	lux::Noise_t fspnParam;
	fspnParam.amplitude = 1.2;
	fspnParam.frequency = 0.666;
	fspnParam.fjump = 1.2;
	fspnParam.octaves = 3.2;
	fspnParam.roughness = 0.6;
	PerlinVelocity velocityField(fspnParam, lux::Vector(0.4, 0.2, 0.8));
	//advection
	SLAdvection advection(&spDensity, &velocityField, 0.5);
	//Mask spAdvect = Mask(&advection);
	SLAdvection advection2(&advection, &velocityField, 0.5);
	SLAdvection advection3(&advection2, &velocityField, 0.5);
	SLAdvection advection4(&advection3, &velocityField, 0.5);
	SLAdvection advection5(&advection4, &velocityField, 0.5);
 	ColorField spAdvectDensCol(lux::Vector(1.0, 1.0, 1.0), &advection5);
	// the array
	SLAdvection* advectedArray = GetAdvectedArray(&bunnyGridDensity, &velocityField, 60);


 	//ColorField spAdvectDensCol(lux::Vector(1.0, 1.0, 1.0), &advection3);
	//dsm grid
	GridPoint dsmSize = { 100, 100, 100 }; // cover 16
	lux::Vector dsmDelta = { 0.16, 0.16, 0.16 };
	lux::Vector dsmLLC(-8.0, -8.0, -8.0);
	GridDetail dsmSetting = { dsmSize, dsmDelta, dsmLLC };
	//lights
	lux::Vector light1Pos(19.30, -5.0, 5.0);
	lux::Vector light2Pos(-19.30, -5.0, 5.0);
	lux::Vector light3Pos(0.0, -5.0, -19.30);
	lux::Vector light1Color(245.0 / 255.0, 66.0 / 255.0, 191.0 / 255.0);
	lux::Vector light2Color(66.0 / 255.0, 84.0 / 255.0, 245.0 / 255.0);
	lux::Vector light3Color(130.0 / 255.0, 240.0 / 255.0, 62.0 / 255.0);

	double lightDeltaS = 0.1;
	double lightKappa = 0.83;
	PointLight light1(light1Color, light1Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 1...\n";
	PointLight light2(light2Color, light2Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 2...\n";
	//PointLight light3(light3Color, light3Pos, dsmSetting, lightDeltaS, lightKappa);
	//std::cout << "light 3...\n";
	
	Renderer rend = Renderer(0.02, 0.7, true);
	rend.AddLight(&light1);
	rend.AddLight(&light2);
	CameraSetup camset;
	lux::Vector cameraLookAt(0.0, 0.0, 0.0);
	camset.position = lux::Vector(0.0, -2.0, 25.0);
	camset.viewDirection = cameraLookAt - camset.position;
	camset.nearPlane = 20.0;
	camset.farPlane = 50.598; // 60.5
	camset.fov = 90.0; // was 100
	Image img(camset, 16 * 120, 9 * 120);
	std::string folderPath = "img5/advection/";
	std::string filename = "";
	ColorField* coll = nullptr;
	//Mask* myMask = nullptr;

	for (int i = imageNum; i < 60; i++)
	{
		std::cout << "Rendering img#" << std::to_string(i) << "...\n";
		ColorField::Clear();
		if (coll) delete coll;
		//
		coll = new ColorField(lux::Vector(1.0, 1.0, 1.0), &advectedArray[i]);
		auto startTime = timeNow;
		light1.CalculateDSM();
		light2.CalculateDSM();
		//
		filename = "bunny_advection_front_l_" + std::to_string(i) + ".exr";
		img.SetupCamera(camset);
		img.CaptureImage(&rend);
		img.WriteImage(folderPath + filename);
		auto endTime = timeNow;
		std::chrono::duration<float> duration = endTime - startTime;
		std::cout << "Elapsed time: " << duration.count() << " seconds\n\n";
	}

	std::cin.get();
	return 0;
}

int mainbunnydisplacement()
{
	std::cout << "Bunny Displacement front\n";
	int imgNumber;
	std::cout << "Enter image number (0 - 59): ";
	std::cin >> imgNumber;
	/////////////////////////
	GridPoint size = { 400, 400, 400 }; //{ 160, 160, 160 }; //{ 80, 80, 80 };
	lux::Vector delta = { 0.01, 0.01, 0.01 }; //{0.11, 0.11, 0.11}; //{ .21, .21, .21 };
	lux::Vector llc(-2.0, -2.0, -2.0);
	GridDetail setting = { size, delta, llc };

	GridPoint dsmSize = { 250, 250, 250 }; // cover 16
	lux::Vector dsmDelta = { 0.064, 0.064, 0.064 };
	lux::Vector dsmLLC(-8.0, -8.0, -8.0);
	GridDetail dsmSetting = { dsmSize, dsmDelta, dsmLLC };

	std::string modelPath = "models/cleanbunny.obj";
	LevelSet bunnyLevelSet(modelPath, setting);
	SFScale bunnyScaled = SFScale(&bunnyLevelSet, 9.2);
	
	SFRotate bunnySR(&bunnyScaled, lux::Vector(1.0, 0.0, 0.0), 180.0);
	SFRotate bunnySR2(&bunnySR, lux::Vector(0.0, -1.0, 0.0), 180.0);
	/*pyro displacement*/
	lux::Noise_t bunnyNoiseParam;
	bunnyNoiseParam.amplitude = 0.06; // 0.06
	bunnyNoiseParam.frequency = 0.666; // 0.666
	bunnyNoiseParam.fjump = 1.2; // 1.2
	bunnyNoiseParam.octaves = 2.2; // 1.2
	bunnyNoiseParam.roughness = 0.5; // 0.5
	
	lux::FractalSum<lux::PerlinNoise> bunnyNoise;
	bunnyNoise.setParameters(bunnyNoiseParam);
	PyroDisplaced bunnyDisplaced = PyroDisplaced(&bunnySR2, &bunnyNoise, 0.8);
	/*------------------------*/
	SpecialMask bunnyDensity(&bunnyDisplaced);
	ColorField spColor(lux::Vector(1.0, 1.0, 1.0), &bunnyDensity);
	
	std::cout << "Levelset loaded..." << std::endl;
	lux::Vector light1Pos(19.30, -5.0, 5.0);
	lux::Vector light2Pos(-19.30, -5.0, 5.0);
	lux::Vector light3Pos(0.0, -5.0, -19.30);
	lux::Vector light1Color(245.0 / 255.0, 66.0 / 255.0, 191.0 / 255.0);
	lux::Vector light2Color(66.0 / 255.0, 84.0 / 255.0, 245.0 / 255.0);
	lux::Vector light3Color(130.0 / 255.0, 240.0 / 255.0, 62.0 / 255.0);

	double lightDeltaS = 0.1;
	double lightKappa = 0.83;
	PointLight light1(light1Color, light1Pos, dsmSetting, lightDeltaS, lightKappa, false);
	std::cout << "light 1...\n";
	PointLight light2(light2Color, light2Pos, dsmSetting, lightDeltaS, lightKappa, false);
	std::cout << "light 2...\n";
	//PointLight light3(light3Color, light3Pos, dsmSetting, lightDeltaS, lightKappa);
	//std::cout << "light 3...\n";

	Renderer rend = Renderer(0.02, 0.7, true);
	rend.AddLight(&light1);
	rend.AddLight(&light2);
	//rend.AddLight(&light3);
	CameraSetup camset;
	lux::Vector cameraLookAt(0.0, 0.0, 0.0);
	camset.position = lux::Vector(0.0, -2.0, 25.0);
	camset.viewDirection = cameraLookAt - camset.position;
	camset.nearPlane = 20.0; // 14.0
	camset.farPlane = 50.598; // 60.598
	camset.fov = 90.0; // was 100
	Image img(camset, 16 * 120, 9 * 120);
	
	double ampInterval = 0.0014;

	std::string folderPath = "img5/displaced/";
	std::string filename;
	//
	img.SetupCamera(camset);

	ColorField* coll = nullptr;
	PyroDisplaced* bunDisp = nullptr;
	SpecialMask* myMask = nullptr;

	for (int i = imgNumber; i < 60; i++)
	{
		// clear
		ColorField::Clear();
		if (coll) delete coll;
		if (bunDisp) delete bunDisp;
		if (myMask) delete myMask;
		// reload
		bunnyNoiseParam.amplitude = i * ampInterval;
		bunnyNoise.setParameters(bunnyNoiseParam);
		std::cout << "Noise Amplitude: " << bunnyNoiseParam.amplitude << std::endl;
		bunDisp = new PyroDisplaced(&bunnySR2, &bunnyNoise, 0.9);
		myMask = new SpecialMask(bunDisp);
		coll = new ColorField(lux::Vector(1.0, 1.0, 1.0), myMask);
		// calculate lights
		auto startTime = timeNow;
		light1.CalculateDSM();
		light2.CalculateDSM();
		// render
		filename = "bunny_displacement_front" + std::to_string(i) + ".exr";
		img.CaptureImage(&rend);
		img.WriteImage(folderPath+filename);
		auto endTime = timeNow;
		//----------------------
		std::chrono::duration<float> duration = endTime - startTime;
		std::cout << "Elapsed time: " << duration.count() << " seconds\n\n";
	}

	/////////////////
	std::cin.get();
	std::cin.get();
	std::cin.get();
	std::cin.get();
	std::cin.get();
	return 0;
}


int main444()
{
	int imgNumber;
	std::cout << "Enter image number (0 - 3): ";
	std::cin >> imgNumber;
	/*
	Sphere sp(2.3);
	lux::Vector spGrad =  sp.grad(lux::Vector(1.0, 0.8, 1.0));
	std::cout << spGrad.X() << ", " << spGrad.Y() << 
		", " << spGrad.Z() << std::endl;

	VFScalarGrad scalarGrad = VFScalarGrad(&sp);
	lux::Vector bla = scalarGrad.eval(lux::Vector(1.0, 0.8, 1.0));
	std::cout << bla.X() << ", " << bla.Y() << ", " << bla.Z() << std::endl;
	
	lux::Noise_t fspn;
	fspn.amplitude = 120.0;
	fspn.frequency = 1.666;
	fspn.fjump = 1.2;
	fspn.octaves = 3.232;
	fspn.roughness = 0.9;

	lux::FractalSum<lux::PerlinNoise> noise;
	noise.setParameters(fspn);
	
	Torus sfTorus(3.0, 1.20, lux::Vector(0.0, 0.0, 1.0));
	Ellipse sfEllipse = Ellipse(2.0, 1.0, lux::Vector(1.0, 0.0, 0.0));
	Cone sfCone(3.5, 15.0, lux::Vector(0.0, 0.0, -2.0), lux::Vector(1.0, 0.0, 0.0));
	PyroDisplaced pyroTorus = PyroDisplaced(&sfTorus, &noise, 0.8, 3);
	Mask torusDens(&pyroTorus);
	ColorField torusCol(lux::Vector(0.5, 0.5, 0.5), &torusDens);
	*/

	// delta kappa
	GridDetail gridLights;
	gridLights.LLC = lux::Vector(-10.1, -10.1, -10.1);
	gridLights.sizeN = { 400, 400, 400 };
	gridLights.delta = lux::Vector(0.0505, 0.0505, 0.0505);

	/*
	ScalarGrid pyroGrid(gridLights, -1000000);
	pyroGrid.Clear(0.0);
	PyroclasticSphere pyroSphere(0.4, 0.8, fspn, gridLights, false);
	pyroSphere.StampInAGrid(&pyroGrid, lux::Vector());
	pyroSphere.StampInAGrid(&pyroGrid, lux::Vector(-3.0, 0.0, 0.0));
	MaskGrid smokeDensity(&pyroGrid);
	ColorField smokeColor = ColorField(lux::Vector(0.5, 0.5, 0.5), &smokeDensity);
	*/
	Kingdom kingdom;
	kingdom.Terrain(); // call this first
	kingdom.Smoke();
	kingdom.LandBridge();
	kingdom.Monument();

	double deltaS = 0.1;
	double kappa = 0.6;
	lux::Vector light1Color(1.0, 1.0, 1.0);
	lux::Vector light1Pos(10.1, -3.0, 5.0);
	PointLight light1(light1Color, light1Pos, gridLights, deltaS, kappa);
	/*camera*/
	CameraSetup camset;
	lux::Vector cameraLookAt(0.0, 0.0, 0.0);
	
	std::vector<lux::Vector> cameraPositions;
	cameraPositions.push_back(lux::Vector(0.0, -15.0, 15.0));
	cameraPositions.push_back(lux::Vector(15.0, -15.0, 0.0));
	cameraPositions.push_back(lux::Vector(0.0, -15.0, -15.0));
	cameraPositions.push_back(lux::Vector(-15.0, -15.0, 0.0));
	
	//camset.position = lux::Vector(0.0, -15.0, 15.0);
	//camset.position = lux::Vector(15.0, -15.0, 0.0);
	//camset.position = lux::Vector(0.0, -15.0, -15.0);
	//camset.position = lux::Vector(-15.0, -15.0, 0.0);

	camset.viewDirection = cameraLookAt - camset.position;
	camset.nearPlane = 10.0;
	camset.farPlane = 50.598; // 60.5
	camset.fov = 80.0; // was 110
	Image img(camset, 16 * 120, 9 * 120);
	Renderer rend = Renderer(0.01, 0.7, true);
	rend.AddLight(&light1);

	/*----*/
	std::string folderpath = "img4/";
	for (int i = imgNumber; i < cameraPositions.size(); i++)
	{
		std::string filename = "kingdom_" + std::to_string(i) + ".exr";
		std::cout << "rendering...\n";
		camset.position = cameraPositions[i];
		camset.viewDirection = cameraLookAt - camset.position;
		//std::cout << cameraPositions[i].X() << ", " << cameraPositions[i].Y() << ", " 
		//	<< cameraPositions[i].Z() << std::endl;
		//std::cin.get();

		img.SetupCamera(camset);
		
		auto startTime = timeNow;
		img.CaptureImage(&rend);
		img.WriteImage(folderpath + filename);
		auto endTime = timeNow;

		std::chrono::duration<float> duration = endTime - startTime;
		std::cout << "Elapsed time: " << duration.count() << " seconds\n\n";
	}

	std::cout << "Done!";
	std::cin.get();
	return 0;
}

int mainWisps() // wisps
{
	std::cout << "Wisps...\n";
	int imgNumuber;
	std::cout << "Enter image number to begin from: (0 - 499) ";
	std::cin >> imgNumuber;

	int clumpOffset;
	std::cout << "Clump offset: (0 - 3): ";
	std::cin >> clumpOffset;
	int freqOffset;
	std::cout << "Freq offset: (0 - 4): ";
	std::cin >> freqOffset;
	int fJumpOffset;
	std::cout << "Fjump offset: (0 - 4): ";
	std::cin >> fJumpOffset;
	int octaveOffset;
	std::cout << "Octave offset: (0 - 4): ";
	std::cin >> octaveOffset;

	ParamControl clumpControl = ParamControl(1.0/3.0, 1.55, 1, clumpOffset); // 0 - 3
	ParamControl freqControl = ParamControl(1.8, 3.2, 5, freqOffset);
	ParamControl fJumpControl = ParamControl(0.7, 1.8, 5, fJumpOffset);
	ParamControl octaveControl = ParamControl(3.151, 10.2, 5, octaveOffset);

	
	double freq;// = freqControl.NextVal();
	double fjmp;// = fJumpControl.NextVal();
	double octv;//= octaveControl.NextVal();
	double clmp;

	std::cout << "Img number: " << imgNumuber << std::endl;
	//std::cin.get();
	/*
	ParamControl a = ParamControl(0.2, 2.5, 100);

	while (true)
	{
		double val = a.NextVal();
		if (val < 0.0) break;
		std::cout << "val: " << val << std::endl;
	}
	std::cin.get();
	*/
	WispsParams wParam;
	wParam.clump = 1.0 / 3.0;
	wParam.delta = 0.1;
	wParam.density = 1.0;

	/*
	FSPNParams fspnWisps[2];
	fspnWisps[0].a = 1.0; fspnWisps[1].a = 1.0;
	fspnWisps[0].f = 0.6; fspnWisps[1].f = 1.6;
	fspnWisps[0].fj = 0.6; fspnWisps[1].fj = 0.5;
	fspnWisps[0].N = 2; fspnWisps[1].N = 3;
	fspnWisps[0].r = 0.1; fspnWisps[1].r = 0.5;
	*/
	lux::Noise_t fspnWisps[2];
	fspnWisps[0].amplitude = 1.2;	fspnWisps[1].amplitude = 1.0;
	fspnWisps[0].frequency = 1.5;	fspnWisps[1].frequency = 1.5;
	fspnWisps[0].fjump = 0.8;		fspnWisps[1].fjump = 1.2;
	fspnWisps[0].octaves = 4.2;		fspnWisps[1].octaves = 3.7;
	fspnWisps[0].roughness = 0.8;	fspnWisps[1].roughness = 0.6;

	GridDetail gridWisps;
	gridWisps.LLC = lux::Vector(-7.5, -7.5, -7.5);
	gridWisps.sizeN = { 550, 550, 550 };
	gridWisps.delta = lux::Vector(0.0273, 0.0273, 0.0273);
	std::cout << "beginning...\n";
	Wisps aWisps(fspnWisps, wParam, gridWisps, lux::Vector(), 5.3, 5000000);
	Mask wispsDens(&aWisps);
	ColorField wispCol(lux::Vector(1.0, 1.0, 1.0), &aWisps);
	/*lights*/
	GridDetail gridDsm;
	gridDsm.LLC = lux::Vector(-7.5, -7.5, -7.5);
	gridDsm.sizeN = { 350, 350, 350 };
	gridDsm.delta = lux::Vector(0.042857, 0.042857, 0.042857);
	std::cout << "light coming up\n";
	// delta kappa
	double delta = 0.05;
	double kappa = 0.83;
	lux::Vector light1Pos(19.30, -5.0, 5.0);
	lux::Vector light2Pos(-19.30, -5.0, 5.0);

	lux::Vector light1Color(245.0 / 255.0, 66.0 / 255.0, 191.0 / 255.0);
	lux::Vector light2Color(66.0 / 255.0, 84.0 / 255.0, 245.0 / 255.0);

	PointLight light1(light1Color, light1Pos, gridDsm, delta, kappa);
	PointLight light2(light2Color, light2Pos, gridDsm, delta, kappa);
	/*--------camera----------*/
	CameraSetup camset;
	lux::Vector cameraLookAt(0.0, 0.0, 0.0);
	camset.position = lux::Vector(0.0, 0.0, 25.0);
	camset.viewDirection = cameraLookAt - camset.position;
	camset.nearPlane = 20.0;
	camset.farPlane = 45.598; // 60.5
	camset.fov = 75.0; // was 110
	Image img(camset, 16 * 60, 9 * 60);
	Renderer rend = Renderer(0.01, 0.7, true);
	rend.AddLight(&light1);
	rend.AddLight(&light2);


	/*----*/
	std::string folderpath = "img3/wisps/";
	//std::cout << "rendering\n";
	
	int counter = imgNumuber;
	Mask* wispDensity = nullptr;
	ColorField* col = nullptr;

	clmp = clumpControl.NextVal();
	do
	{
		freq = freqControl.NextVal();
		do
		{
			fjmp = fJumpControl.NextVal();
			do
			{
				octv = octaveControl.NextVal();
				do
				{
					// do everything here
					//clear
					ColorField::Clear();
					if (wispDensity) delete wispDensity;
					if (col) delete col;
					//new settings

					aWisps.SetClump(clmp);
					aWisps.SetFJump(fjmp);
					aWisps.SetFreq1(freq);
					aWisps.SetOctaves(octv);
					//new density, and dsm
					aWisps.CalculateWisps();
					//wispDensity = new Mask(&aWisps);
					col = new ColorField(lux::Vector(1.0, 1.0, 1.0), &aWisps);
					light1.CalculateDSM();
					light2.CalculateDSM();
					// render
					img.CaptureImage(&rend);
					std::string fileName = "wispnew_" + std::to_string(counter) + ".exr";
					img.WriteImage(folderpath + fileName);
					
					/////////////////
					counter++;
					std::cout << "Done with offsets, clmp, freq, fjmp, octv...\n";
					std::cout << clumpControl.GetOffset() << std::endl;
					std::cout << freqControl.GetOffset() << std::endl;
					std::cout << fJumpControl.GetOffset() << std::endl;
					std::cout << octaveControl.GetOffset() << std::endl << std::endl;
					octv = octaveControl.NextVal();
				} while (octv > 0.0); octaveControl.Reset();

				fjmp = fJumpControl.NextVal();
			} while (fjmp > 0.0); fJumpControl.Reset();

			freq = freqControl.NextVal();
		} while (freq > 0.0); freqControl.Reset();

		clmp = clumpControl.NextVal();
	} while (clmp > 0.0);

	/*
	do
	{
		img.CaptureImage(&rend);
		std::string fileName = "nomask" + std::to_string(counter) + ".exr";
		img.WriteImage(folderpath + fileName);
		//clear
		ColorField::Clear();
		if (wispDensity) delete wispDensity;
		if (col) delete col;
		//new settings
		jump1 += inc;
		aWisps.SetFJump(jump1);
		counter++;
		//new density, and dsm
		aWisps.CalculateWisps();
		//wispDensity = new Mask(&aWisps);
		col = new ColorField(lux::Vector(1.0, 1.0, 1.0), &aWisps);
		light1.CalculateDSM();
		light2.CalculateDSM();
	} while (jump1 <= 2.5);
	*/
	std::cin.get();
	return 0;
}

int mainNS() // noise stamping
{
	/*
	{
		std::mt19937 generator;
		std::uniform_real_distribution<double> uniform(-1.0, 1.0);
	
		for (int i = 0; i < 500; i++)
		{
			std::cout << uniform(generator) << std::endl;
		}

		std::cin.get();
	}
	{
		Sphere sp = Sphere(4.0);
		Mask mskSp = Mask(&sp);
		ColorField spCol = ColorField(lux::Vector(1.0, 0.0, 0.0), &mskSp);

		GridDetail grd;
		grd.LLC = lux::Vector(-5.0, -5.0, -5.0);
		grd.delta = lux::Vector(0.1 , 0.1, 0.1);
		grd.sizeN = { 100, 100, 100 };
		PointLight pl (lux::Vector(), lux::Vector(20.0, 0.0, 0.0), grd, 0.01, 1.0);
		
		auto startTime = timeNow;
		pl.CalculateDSM();
		auto endTime = timeNow;
		std::chrono::duration<float> duration = endTime - startTime;
		std::cout << "Elapsed time: " << duration.count() << " seconds\n";
		//pl.DebugDSM();
		
		//multithreading
		auto startTimeMT = timeNow;
		pl.CalculateDSMmt2();
		auto endTimeMT = timeNow;
		std::chrono::duration<float> durationMT = endTimeMT - startTimeMT;
		std::cout << "Elapsed time for MT: " << durationMT.count() << " seconds\n";

		//pl.DebugDSM();
		std::cin.get();
	}
	*/

	std::cout << "Stamp noise...\n";
	int imgNumuber;
	std::cout << "Enter image number to begin from: (0 - 499) ";
	std::cin >> imgNumuber;

	int fadeOffset;
	std::cout << "Fade offset: (0 - 3): ";
	std::cin >> fadeOffset;
	int freqOffset;
	std::cout << "Freq offset: (0 - 4): ";
	std::cin >> freqOffset;
	int fJumpOffset;
	std::cout << "Fjump offset: (0 - 4): ";
	std::cin >> fJumpOffset;
	int octaveOffset;
	std::cout << "Octave offset: (0 - 4): ";
	std::cin >> octaveOffset;


	ParamControl fadeControl = ParamControl(0.34, 0.82, 4, fadeOffset); // 0 - 3
	ParamControl freqControl = ParamControl(1.7, 3.2, 5, freqOffset);
	ParamControl fJumpControl = ParamControl(1.5, 2.8, 5, fJumpOffset);
	ParamControl octaveControl = ParamControl(2.11, 10.2, 5, octaveOffset);


	double fde;
	double freq;// = freqControl.NextVal();
	double fjmp;// = fJumpControl.NextVal();
	double octv;//= octaveControl.NextVal();
	/*----------------------------*/
	lux::Noise_t fspnNoiseStamp;
	fspnNoiseStamp.amplitude = 4.0;
	fspnNoiseStamp.frequency = 2.666;
	fspnNoiseStamp.fjump = 1.2;
	fspnNoiseStamp.octaves = 2.2;
	fspnNoiseStamp.roughness = 0.9;

	NoiseStampParams nsParams;
	nsParams.pc = lux::Vector();
	nsParams.fade = 0.5;
	nsParams.pScale = 4.9;

	GridDetail gridNS;
	gridNS.LLC = lux::Vector(-5.0, -5.0, -5.0);
	gridNS.sizeN = { 500, 500, 500 };
	gridNS.delta = lux::Vector(0.02, 0.02, 0.02);

	NoiseStamp noiseStamp(fspnNoiseStamp, nsParams, gridNS, false);
	ColorField nsCol = ColorField(lux::Vector(1.0, 1.0, 1.0), &noiseStamp);
	/*---------light---------*/
	GridDetail gridLights;
	gridLights.LLC = lux::Vector(-5.0, -5.0, -5.0);
	gridLights.sizeN = { 330, 330, 330 };
	gridLights.delta = lux::Vector(0.03125, 0.03125, 0.03125);
	//lux::Vector light1Pos(16.30, -5.0, 5.0);
	//lux::Vector light1Color(0.6, 0.4, 0.51);
	lux::Vector light1Pos(19.30, -5.0, 5.0);
	lux::Vector light2Pos(-19.30, -5.0, 5.0);

	lux::Vector light1Color(245.0 / 255.0, 66.0 / 255.0, 191.0 / 255.0);
	lux::Vector light2Color(66.0 / 255.0, 84.0 / 255.0, 245.0 / 255.0);

	std::cout << "light 1...\n";
	PointLight light1(light1Color, light1Pos, gridLights, 0.1, 0.6);
	//light 2
	std::cout << "light 2...\n";
	PointLight light2(light2Color, light2Pos, gridLights, 0.1, 0.6);
	/*--------camera---------*/
	CameraSetup camset;
	lux::Vector cameraLookAt(0.0, 0.0, 0.0);
	camset.position = lux::Vector(0.0, 0.0, 25.0);
	camset.viewDirection = cameraLookAt - camset.position;
	camset.nearPlane = 20.0; //14
	camset.farPlane = 46.598; // 60.5
	camset.fov = 75.0; // was 110
	Image img(camset, 16 * 60, 9 * 60);
	Renderer rend = Renderer(0.01, 0.7, true);
	rend.AddLight(&light1);
	rend.AddLight(&light2);

	/*rendering*/
	//std::cout << "Rendering...\n";
	//img.CaptureImage(&rend);
	//img.WriteImage("img3/ns/stampNoise6.exr");
	/*----*/
	std::string folderpath = "img3/ns/";
	//std::cout << "rendering\n";

	int counter = imgNumuber;
	Mask* wispDensity = nullptr;
	ColorField* col = nullptr;

	fde = fadeControl.NextVal();
	do
	{
		freq = freqControl.NextVal();
		do
		{
			fjmp = fJumpControl.NextVal();
			do
			{
				octv = octaveControl.NextVal();
				do
				{
					// do everything here
					//clear
					ColorField::Clear();
					if (wispDensity) delete wispDensity;
					if (col) delete col;
					//new settings
					noiseStamp.SetFade(fde);
					noiseStamp.SetFJump(fjmp);
					noiseStamp.SetFreq(freq);
					noiseStamp.SetOctaves(octv);
					//new density, and dsm
					noiseStamp.StoreInGrid();
					//wispDensity = new Mask(&aWisps);
					col = new ColorField(lux::Vector(1.0, 1.0, 1.0), &noiseStamp);
					light1.CalculateDSM();
					light2.CalculateDSM();
					// render
					img.CaptureImage(&rend);
					std::string fileName = "noiseStamp_" + std::to_string(counter) + ".exr";
					img.WriteImage(folderpath + fileName);

					/////////////////
					counter++;
					std::cout << "Done with fade, clmp, freq, fjmp, octv...\n";
					std::cout << fadeControl.GetOffset() - 1 << std::endl;
					std::cout << freqControl.GetOffset() - 1<< std::endl;
					std::cout << fJumpControl.GetOffset() - 1<< std::endl;
					std::cout << octaveControl.GetOffset() -1<< std::endl << std::endl;
					octv = octaveControl.NextVal();
				} while (octv > 0.0); octaveControl.Reset();

				fjmp = fJumpControl.NextVal();
			} while (fjmp > 0.0); fJumpControl.Reset();

			freq = freqControl.NextVal();
		} while (freq > 0.0); freqControl.Reset();

		fde = fadeControl.NextVal();
	} while (fde > 0.0);

	std::cin.get();
	return 0;
}

int mainPyro() // pyroclastic wedge
{
	/*
	FSPNParams fspnPyro;
	fspnPyro.a = 1.0;
	fspnPyro.f = 0.7;
	fspnPyro.fj = 1.2;
	fspnPyro.N = 3;
	fspnPyro.r = 0.9;
	*/

	int imgNumuber;
	std::cout << "Enter image number to begin from: (0 - 499) ";
	std::cin >> imgNumuber;

	int gammaOffset;
	std::cout << "gamma offset: (0 - 4): ";
	std::cin >> gammaOffset;
	int freqOffset;
	std::cout << "Freq offset: (0 - 4): ";
	std::cin >> freqOffset;
	int fJumpOffset;
	std::cout << "Fjump offset: (0 - 4): ";
	std::cin >> fJumpOffset;
	int octaveOffset;
	std::cout << "Octave offset: (0 - 3): ";
	std::cin >> octaveOffset;

	ParamControl gammaControl = ParamControl(0.4, 1.2, 3, gammaOffset); // 0 - 4
	ParamControl freqControl = ParamControl(0.5, 1.51, 5, freqOffset);
	ParamControl fJumpControl = ParamControl(0.34, 1.78, 5, fJumpOffset);
	ParamControl octaveControl = ParamControl(3.7, 9.2, 3, octaveOffset); // 0 - 3


	lux::Noise_t fspnNewPyro;
	fspnNewPyro.amplitude = 2.0;
	fspnNewPyro.frequency = 0.666;
	fspnNewPyro.fjump = 2.0;
	fspnNewPyro.octaves = 3.2;
	fspnNewPyro.roughness = 0.9;

	//lux::FractalSum<lux::PerlinNoise> fs;
	//std::cout <<  fs.eval(lux::Vector(0.0, 1.0, 2.0));

	GridDetail gridPyro;
	gridPyro.LLC = lux::Vector(-7.0, -7.0, -7.0);
	gridPyro.sizeN = { 500, 500, 500 };
	gridPyro.delta = lux::Vector(0.028, 0.028, 0.028);
	PyroclasticSphere pyroSp(4.0, 2.2, fspnNewPyro, gridPyro, false);
	Mask msk = Mask(&pyroSp);
	ColorField pyroCol = ColorField(lux::Vector(0.8, 0.8, 0.8), &msk);
	/*--------light-----------*/
	GridDetail gridLights;
	gridLights.LLC = lux::Vector(-7.0, -7.0, -7.0);
	gridLights.sizeN = { 320, 320, 320 };
	gridLights.delta = lux::Vector(0.04375, 0.04375, 0.04375);
	//lux::Vector light1Pos(16.30, -5.0, 5.0);
	//lux::Vector light1Color(0.6, 0.4, 0.51);
	lux::Vector light1Pos(19.30, -5.0, 5.0);
	lux::Vector light2Pos(-19.30, -5.0, 5.0);

	lux::Vector light1Color(245.0 / 255.0, 66.0 / 255.0, 191.0 / 255.0);
	lux::Vector light2Color(66.0 / 255.0, 84.0 / 255.0, 245.0 / 255.0);

	std::cout << "light 1...\n";
	PointLight light1(light1Color, light1Pos, gridLights, 0.1, 0.6);
	//light 2
	std::cout << "light 2...\n";
	PointLight light2(light2Color, light2Pos, gridLights, 0.1, 0.6);
	std::cout << "rendering...\n";
	/*--------camera----------*/
	CameraSetup camset;
	lux::Vector cameraLookAt(0.0, 0.0, 0.0);
	camset.position = lux::Vector(0.0, 0.0, 25.0);
	camset.viewDirection = cameraLookAt - camset.position;
	camset.nearPlane = 20.0; //14
	camset.farPlane = 45.598; // 60.5
	camset.fov = 75.0; // was 110
	Image img(camset, 16 * 60, 9 * 60);
	Renderer rend = Renderer(0.01, 0.7, true);
	rend.AddLight(&light1);
	rend.AddLight(&light2);
	
	
	//img.CaptureImage(&rend);
	//img.WriteImage("img3/pyro/pyro6.exr");

	double gamma, freq, fjmp, octv;
	int counter = imgNumuber;
	Mask* pyroDensity = nullptr;
	ColorField* col = nullptr;
	std::string folderpath = "img3/pyro/";


	octv = octaveControl.NextVal();
	do
	{
		freq = freqControl.NextVal();
		do
		{
			fjmp = fJumpControl.NextVal();
			do
			{
				gamma = gammaControl.NextVal();
				do
				{
					// do everything here
					//clear
					ColorField::Clear();
					if (pyroDensity) delete pyroDensity;
					if (col) delete col;
					//new settings

					pyroSp.SetGamma(gamma);
					pyroSp.SetFJump(fjmp);
					pyroSp.SetFreq(freq);
					pyroSp.SetOctaves(octv);
					//new density, and dsm
					pyroSp.StoreInGrid();
					pyroDensity = new Mask(&pyroSp);
					col = new ColorField(lux::Vector(1.0, 1.0, 1.0), pyroDensity);
					light1.CalculateDSM();
					light2.CalculateDSM();
					// render
					img.CaptureImage(&rend);
					std::string fileName = "pyronew_" + std::to_string(counter) + ".exr";
					img.WriteImage(folderpath + fileName);

					/////////////////
					counter++;
					std::cout << "Done with offsets, gamma, freq, fjmp, octv...\n";
					std::cout << gammaControl.GetOffset() - 1 << std::endl;
					std::cout << freqControl.GetOffset() - 1 << std::endl;
					std::cout << fJumpControl.GetOffset() - 1 << std::endl;
					std::cout << octaveControl.GetOffset() - 1<< std::endl << std::endl;
					gamma = gammaControl.NextVal();
				} while (gamma > 0.0); gammaControl.Reset();

				fjmp = fJumpControl.NextVal();
			} while (fjmp > 0.0); fJumpControl.Reset();

			freq = freqControl.NextVal();
		} while (freq > 0.0); freqControl.Reset();

		octv = octaveControl.NextVal();
	} while (octv > 0.0);

	std::cout << "DONE!\n";

	std::cin.get();
	return 0;
}

int mainAvatarTea() // avatar teapot
{
	/*for teapot*/
	GridPoint size = { 300, 300, 300 };
	lux::Vector delta = { 0.02, 0.02, 0.02 };
	lux::Vector llc(-3.0, -3.0, -3.0);
	GridDetail setting = { size, delta, llc };

	/*for bunny*/
	GridPoint size2 = { 300, 300, 300 }; //{ 160, 160, 160 }; //{ 80, 80, 80 };
	lux::Vector delta2 = { 0.01333, 0.01333, 0.01333 }; //{0.11, 0.11, 0.11}; //{ .21, .21, .21 };
	lux::Vector llc2(-2.0, -2.0, -2.0);
	GridDetail setting2 = { size2, delta2, llc2 };

	/*dsm*/
	GridPoint dsmSize = { 400, 420, 400 }; 
	lux::Vector dsmDelta = { 0.075, 0.09, 0.085 };
	lux::Vector dsmLLC(-15.0, -14.0, -12.0);
	GridDetail dsmSetting = { dsmSize, dsmDelta, dsmLLC };

	int nrOfImages = 120;
	int beginFrom = 0;
	std::cout << "Image number to begin from (0-" << nrOfImages - 1 << "): ";
	std::cin >> beginFrom;

	/* bunny and teapot translate amount */
	lux::Vector parentTranslate(-7.8, 5.0, 13.65);

	std::string modelPath = "models/cleanteapot.obj";
	LevelSet boxLevelSet(modelPath, setting);
	SFScale boxScaled = SFScale(&boxLevelSet, 3.0); // 9.0
	SFRotate boxSR(&boxScaled, lux::Vector(1.0, 0.0, 0.0), 180.0);
	SFRotate boxSR2(&boxSR, lux::Vector(0.0, 1.0, 0.0), -90.0);
	//SFTranslate lsTrans(&boxLevelSet, lux::Vector(0.0, -1.0, 0.0));
	Plane plane = Plane(lux::Vector(0.0, 1.0, 0.0), lux::Vector());
	SFTranslate planeT = SFTranslate(&plane, lux::Vector(0.0, -1.167, 0.0)); // -3.5

	Cutout boxSRMPlane = Cutout(&boxSR2, &planeT);
	SFTranslate boxSRMPlaneT = SFTranslate(&boxSRMPlane, parentTranslate);
	
	Mask boxDensity(&boxSRMPlaneT);
	ColorField spColor(lux::Vector(0.4, 0.4, 0.4), &boxDensity);

	std::string modelPath2 = "models/cleanbunny.obj";
	LevelSet bunnyLevelSet(modelPath2, setting2);
	SFScale bunnyScaled = SFScale(&bunnyLevelSet, 2.67); // 8.0 / 3
	SFRotate bunnySR = SFRotate(&bunnyScaled, lux::Vector(1.0, 0.0, 0.0), 180.0);
	SFTranslate bunnySRT = SFTranslate(&bunnySR, lux::Vector(0.0, -1.167, 0.4) + parentTranslate); // change
	Mask bunnyDensity = Mask(&bunnySRT);
	ColorField bunnyColor(lux::Vector(0.5, 0.5, 0.5), &bunnyDensity);
	


	//Sphere sp = Sphere(4.0);
	//Mask spDensity = Mask(&sp);
	//ColorField spColor = ColorField(lux::Vector(1.0, 1.0, 1.0), &spDensity);

	Avatar humanoid;
	humanoid.FinalAvatar();

	std::cout << "Levelset loaded..." << std::endl;

	lux::Vector light1Pos(30.30, -18.0, 15.0 );
	lux::Vector light2Pos(-30.30, -18.0, 15.0 ) ;
	lux::Vector light3Pos(0.0, -18.0, -30.30);

	lux::Vector light1Color(245.0 / 255.0, 245.0 / 255.0, 245.0 / 255.0);
	lux::Vector light2Color(255.0 / 255.0, 245.0 / 255.0, 245.0 / 255.0);
	//lux::Vector light2Color(255.0 / 255.0, 45.0 / 255.0, 45.0 / 255.0);
	lux::Vector light3Color(245.0 / 255.0, 245.0 / 255.0, 245.0 / 255.0);

	double lightDeltaS = 0.1; // 0.1
	double lightKappa = 0.83;
	PointLight light1(light1Color, light1Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 1\n";
	PointLight light2(light2Color, light2Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 2...\n";
	PointLight light3(light3Color, light3Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 3...\n";

	Renderer rend = Renderer(0.02, 0.73, true); // 0.02
	rend.AddLight(&light1);
	rend.AddLight(&light2);
	rend.AddLight(&light3);
	//
	CameraSetup camset;
	camset.position = lux::Vector(10.0, 0.0, 0.0);
	camset.viewDirection = lux::Vector(0.0, 5.0, 0.0) - camset.position;
	camset.nearPlane = 13.0;
	camset.farPlane = 87.598; // 60.5
	camset.fov = 110.0; // was 110
	Image img(camset, 16 * 120, 9 * 120);


	/*camera turntable*/
	double degreeInterval = 360.0 / (double)nrOfImages; // 
	double radius = 30.0; // 25
	double currentAngle = 0.0;
	//lux::Vector lookAt = lux::Vector(0.0, 4.98, 0.0);
	lux::Vector lookAt = lux::Vector(0.0, 4.22, 0.0);
	std::string imgName = "humanoidtealitfin";
	//std::string imgName = "cameraTest";

	std::string folderName = "img2/humanoid";
	
	for (int i = beginFrom; i < nrOfImages; i++)
	{
		std::cout << "Generating image#" << i << "..." << std::endl;
		currentAngle = (i * degreeInterval); // mult i, final

		double z = radius * std::cos(DegToRad(currentAngle));
		double x = radius * std::sin(DegToRad(currentAngle));
		camset.position = lux::Vector(x, -2.0, z);
		camset.viewDirection = lookAt - camset.position;
		img.SetupCamera(camset);

		std::string filename = imgName + std::to_string(i);
		filename = folderName + "/" + filename + ".exr";
		//PrintVector("camera position", camset.position);
		auto startTime = timeNow;
		img.CaptureImage(&rend);
		img.WriteImage(filename);
		auto endTime = timeNow;
		std::chrono::duration<float> duration = endTime - startTime;
		std::cout << "Elapsed time: " << duration.count() << " seconds\n\n";
	}

	std::cout << "Done!" << std::endl;

	std::cin.get();
	return 0;
}


int mainbunnyteapot() /* bunny teapot */
{
	/*for teapot*/
	GridPoint size = { 400, 400, 400 }; 
	lux::Vector delta = { 0.015, 0.015, 0.015 }; 
	lux::Vector llc(-3.0, -3.0, -3.0);
	GridDetail setting = { size, delta, llc };

	/*for bunny*/
	GridPoint size2 = { 400, 400, 400 }; //{ 160, 160, 160 }; //{ 80, 80, 80 };
	lux::Vector delta2 = { 0.01, 0.01, 0.01 }; //{0.11, 0.11, 0.11}; //{ .21, .21, .21 };
	lux::Vector llc2(-2.0, -2.0, -2.0);
	GridDetail setting2 = { size2, delta2, llc2 };

	GridPoint dsmSize = { 400, 400, 400 }; // cover 27
	lux::Vector dsmDelta = { 0.0675, 0.0675, 0.0675 };
	lux::Vector dsmLLC(-13.5, -13.5, -13.5);
	GridDetail dsmSetting = { dsmSize, dsmDelta, dsmLLC };

	int nrOfImages = 120;
	int beginFrom = 0;
	std::cout << "Image number to begin from (0-" << nrOfImages - 1 << "): ";
	std::cin >> beginFrom;
	
	std::string modelPath = "models/cleanteapot.obj";
	LevelSet boxLevelSet(modelPath, setting);
	SFScale boxScaled = SFScale(&boxLevelSet, 9.0);
	SFRotate boxSR(&boxScaled, lux::Vector(1.0, 0.0, 0.0), 180.0);
	//SFTranslate lsTrans(&boxLevelSet, lux::Vector(0.0, -1.0, 0.0));
	Plane plane = Plane(lux::Vector(0.0, 1.0, 0.0), lux::Vector());
	SFTranslate planeT = SFTranslate(&plane, lux::Vector(0.0, -3.5, 0.0)); 
	Cutout boxSRMPlane = Cutout(&boxSR, &planeT);

	Mask boxDensity(&boxSRMPlane);
	ColorField spColor(lux::Vector(0.4, 0.4, 0.4), &boxDensity);
	
	std::string modelPath2 = "models/cleanbunny.obj";
	LevelSet bunnyLevelSet(modelPath2, setting2);
	SFScale bunnyScaled = SFScale(&bunnyLevelSet, 8.0);
	SFRotate bunnySR = SFRotate(&bunnyScaled, lux::Vector(1.0, 0.0, 0.0), 180.0);
	SFTranslate bunnySRT = SFTranslate(&bunnySR, lux::Vector(0.0, -3.5, 0.4));
	Mask bunnyDensity = Mask(&bunnySRT);
	ColorField bunnyColor(lux::Vector(0.5, 0.5, 0.5), &bunnyDensity);
	
	//Sphere sp = Sphere(4.0);
	//Mask spDensity = Mask(&sp);
	//ColorField spColor = ColorField(lux::Vector(1.0, 1.0, 1.0), &spDensity);

	std::cout << "Levelset loaded..." << std::endl;
	lux::Vector light1Pos(30.30, -18.0, 15.0);
	lux::Vector light2Pos(-30.30, -18.0, 15.0);
	lux::Vector light3Pos(0.0, -18.0, -30.30);
	lux::Vector light1Color(245.0 / 255.0, 66.0 / 255.0, 191.0 / 255.0);
	lux::Vector light2Color(66.0 / 255.0, 84.0 / 255.0, 245.0 / 255.0);
	lux::Vector light3Color(130.0 / 255.0, 240.0 / 255.0, 62.0 / 255.0);

	double lightDeltaS = 0.1;
	double lightKappa = 0.83;
	PointLight light1(light1Color, light1Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 1\n";
	PointLight light2(light2Color, light2Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 2...\n";
	PointLight light3(light3Color, light3Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 3...\n";

	Renderer rend = Renderer(0.01, 0.73, true);
	rend.AddLight(&light1);
	rend.AddLight(&light2);
	rend.AddLight(&light3);
	//
	CameraSetup camset;
	camset.position = lux::Vector(10.0, 0.0, 0.0);
	camset.viewDirection = lux::Vector(0.0, 5.0, 0.0) - camset.position;
	camset.nearPlane = 14.0;
	camset.farPlane = 60.598; // 60.5
	camset.fov = 100.0; // was 110
	Image img(camset, 16 * 120, 9 * 120);


	/*camera turntable*/

	double degreeInterval = 360.0 / (double)nrOfImages; // 
	double radius = 25.0;
	double currentAngle = 0.0;
	//lux::Vector lookAt = lux::Vector(0.0, 4.98, 0.0);
	lux::Vector lookAt = lux::Vector(0.0, 0.00, 0.0);
	std::string imgName = "teapotbunny";
	std::string folderName = "img2/teapot";

	for (int i = beginFrom; i < nrOfImages; i++)
	{
		std::cout << "Generating image#" << i << "..." << std::endl;
		currentAngle = (i * degreeInterval); // mult i, final

		double z = radius * std::cos(DegToRad(currentAngle));
		double x = radius * std::sin(DegToRad(currentAngle));
		camset.position = lux::Vector(x, -6.0, z);
		camset.viewDirection = lookAt - camset.position;
		img.SetupCamera(camset);

		std::string filename = imgName + std::to_string(i);
		filename = folderName + "/" + filename + ".exr";
		//PrintVector("camera position", camset.position);
		img.CaptureImage(&rend);
		img.WriteImage(filename);
	}

	std::cout << "Done!" << std::endl;


	std::cin.get();
	return 0;
}

int mainbunny() // level set bunny
{/*
	Cone c(2.1, 2, lux::Vector(), lux::Vector());

	{
		using limits = std::numeric_limits<double>;
		double d = limits::infinity();

		std::cout << d << std::endl;
		// Initialize Loader
		objl::Loader Loader;

		// Load .obj File
		bool loadout = Loader.LoadFile("models/bunny.obj");

		if (loadout)
		{
			std::cout << "loaded\n";
		}

		lux::Vector x(0.0, 0.0, -1.0);
		lux::Vector p1(1.0, 0.0, 0.0);
		lux::Vector p2(0.0, 1.0, 0.0);
		lux::Vector p3(0.0, 0.0, 0.0);
		if (pointInTriangle(x, p1, p2, p3))
		{
			std::cout << "projecting\n";
		}
		else
		{
			std::cout << "not projecting\n";
		}
		std::cin.get();
	}
	
	GridPoint size = { 40, 30, 20 };
	lux::Vector delta = { .02, .02, .02 };
	ScalarGrid grid({ size, delta, {0.0, 0.0, 0.0} }, 0.0);

	LevelSet balLevelSet("afsdf", grid.GetSetting());
	std::cout << balLevelSet.eval(lux::Vector());
	std::cin.get();

	grid.SetGridData({10, 20, 3 }, 42);
	//std::cout << grid.GetGridData({ 1, 2, 3 }) << std::endl;
	//std::cout << grid.eval(grid.GridPointToVector({10, 20, 3})) << std::endl;
	*/
	
	//ModelLoader mdl = ModelLoader("models/plane.obj");
	//mdl.Debug();
	
	/*
	lux::Vector ps(-.60, -0.2, -0.2);
	lux::Vector d(1.0, 0.0, 0.0);
	d.normalize();
	lux::Vector p0(-0.5, 0.5, 0.5);
	lux::Vector p1(-0.5, 0.5, -0.5);
	lux::Vector p2(-0.5, -0.5, 0.5);
	
	lux::Vector ri;
	
	if (RayIntersectsTriangle(ps, d, p0, p1, p2, ri))
	{
		std::cout << "Intersect\n";
	}
	else
	{
		std::cout << "No intersect\n";
	}

	std::cin.get();
	*/
	GridPoint size = { 400, 400, 400 }; //{ 160, 160, 160 }; //{ 80, 80, 80 };
	lux::Vector delta = { 0.01, 0.01, 0.01 }; //{0.11, 0.11, 0.11}; //{ .21, .21, .21 };
	lux::Vector llc(-2.0, -2.0, -2.0);
	GridDetail setting = { size, delta, llc };
	
	GridPoint dsmSize = { 400, 400, 400 }; // cover 16
	lux::Vector dsmDelta = { 0.04, 0.04, 0.04 };
	lux::Vector dsmLLC(-8.0, -8.0, -8.0);
	GridDetail dsmSetting = { dsmSize, dsmDelta, dsmLLC };

	//LevelSet lsPlane ("models/plane.obj", setting);
	//std::cin.get();
	//ScalarGrid grd(setting);
	//grd.SetGridData({ 10, 20, 3 }, 42);
	//std::cout << grd.GetGridData({ 10, 20, 3 }) << std::endl;
	//std::cout << grd.eval(grd.GridPointToVector({ 0, 59, 0 })) << std::endl;
	//std::cout << grd.eval(lux::Vector());
	//std::cin.get();
	
	int nrOfImages = 120;
	int beginFrom = 0;
	std::cout << "Image number to begin from (0-" << nrOfImages - 1 << "): ";
	std::cin >> beginFrom;

	std::string modelPath = "models/cleanbunny.obj";
	LevelSet boxLevelSet(modelPath, setting);
	SFScale boxScaled = SFScale(&boxLevelSet, 9.2);
	SFRotate boxSR(&boxScaled, lux::Vector(1.0, 0.0, 0.0), 180.0);
	SFTranslate lsTrans(&boxLevelSet, lux::Vector(0.0, -1.0, 0.0));
	Mask boxDensity(&boxSR);
	ColorField spColor(lux::Vector(1.0, 1.0, 1.0), &boxDensity);
	
	std::cout << "Levelset loaded..." << std::endl;
	lux::Vector light1Pos(19.30, -5.0, 5.0);
	lux::Vector light2Pos(-19.30, -5.0, 5.0);
	lux::Vector light3Pos(0.0, -5.0, -19.30);
	lux::Vector light1Color(245.0 / 255.0, 66.0 / 255.0, 191.0 / 255.0);
	lux::Vector light2Color(66.0/255.0, 84.0/ 255.0, 245.0/ 255.0);
	lux::Vector light3Color(130.0/255.0, 240.0/255.0, 62.0/255.0);
	/////
	//Sphere sp(6.0);
	//Mask spDen(&sp);
	//ColorField spColor(lux::Vector(1.0, 1.0, 1.0), &spDen);
	// deltaS, Kappa
	double lightDeltaS = 0.1;
	double lightKappa = 0.83;
	PointLight light1(light1Color, light1Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 1\n";
	PointLight light2(light2Color, light2Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 2...\n";
	PointLight light3(light3Color, light3Pos, dsmSetting, lightDeltaS, lightKappa);
	std::cout << "light 3...\n";

	Renderer rend = Renderer(0.01, 0.7, true);
	rend.AddLight(&light1);
	rend.AddLight(&light2);
	rend.AddLight(&light3);
	//
	CameraSetup camset;
	camset.position = lux::Vector(10.0, 0.0, 0.0);
	camset.viewDirection = lux::Vector(0.0, 5.0, 0.0) - camset.position;
	camset.nearPlane = 14.0;
	camset.farPlane = 60.598; // 60.5
	camset.fov = 100.0; // was 110
	Image img(camset, 16 * 120, 9 * 120);


	/*camera turntable*/

	double degreeInterval = 360.0 / (double)nrOfImages; // 
	double radius = 25.0;
	double currentAngle = 0.0;
	//lux::Vector lookAt = lux::Vector(0.0, 4.98, 0.0);
	lux::Vector lookAt = lux::Vector(0.0, 0.00, 0.0);
	std::string imgName = "bunny__final";
	std::string folderName = "img2/teapot";

	

	for (int i = beginFrom; i < nrOfImages; i++)
	{
		std::cout << "Generating image#" << i << "..." << std::endl;
		currentAngle = (i * degreeInterval); // mult i, final

		double z = radius * std::cos(DegToRad(currentAngle));
		double x = radius * std::sin(DegToRad(currentAngle));
		camset.position = lux::Vector(x, 0.0, z);
		camset.viewDirection = lookAt - camset.position;
		img.SetupCamera(camset);

		std::string filename = imgName + std::to_string(i);
		filename = folderName + "/" + filename + ".exr";
		//PrintVector("camera position", camset.position);
		img.CaptureImage(&rend);
		img.WriteImage(filename);
	}

	std::cout << "Done!" << std::endl;

	std::cin.get();
	return 0;
}

int main1()
{
	/*
	lux::Matrix mat = lux::Matrix();
	mat.det();
	mat = lux::Matrix::OuterProduct(lux::Vector(1.0, 2.0, 3.0), lux::Vector(-1.0, 0.0, 2.0));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << mat(i, j) << " ";
		}
		std::cout << std::endl;
	}

	//std::cin.get();
	
	lux::Vector aVec = lux::Vector(3.0, 1.0f, 2.0);
	lux::Vector bVec = lux::Vector();

	bVec = aVec.unitvector();
	bVec[0] = 1.0;

	std::cout << bVec[0] << std::endl;
	
	{
		I a(2), b(3);
		a = 21; // ???
		b = a + a;
		std::cout << b.value << std::endl;
	}

	Sphere sp1 = Sphere(5.0f);
	lux::Vector v = lux::Vector(0.0, 1.0, 20.0);
	std::cout << sp1.eval(v) << std::endl;
	Ellipse el1 = Ellipse(5.0, 2.0, v);
	std::cout << el1.eval(v) << std::endl;

	lux::Volume<double>* vol;
	vol = &sp1;
	std::cout << vol->eval(v) << std::endl;
	vol = &el1;
	std::cout << vol->eval(v) << std::endl;
	
	Union sphereandellipse = Union(&sp1, &el1);
	vol = &sphereandellipse;
	std::cout << vol->eval(v) << std::endl;

	
	Sphere sp11 = Sphere(20);
	Sphere sp12 = Sphere(30);

	ColorField cf = ColorField(lux::Vector(100, 10, 20), &sp11);
	ColorField cf2 = ColorField(lux::Vector(100, 10, 20), &sp12);

	ColorField::eval(lux::Vector(0, 0, 1));
	
	//CameraSetup ball;
	//std::cout << ball.fov << std::endl;
	Sphere sphe = Sphere(2.0);
	std::cout << sphe.eval(lux::Vector(0.0, 1.1, 2.0)) << std::endl;

	Ray ray;
	ray.direction = lux::Vector(0.0, 0.0, 1.0);
	ray.origin = lux::Vector(0.0, 0.0, -6.0);
	ray.sBegin = -5.5;
	ray.sEnd = 5.5;
	
	Ellipse ellp = Ellipse(2.1, 1.2, lux::Vector(0.0, 1.0, 0.0));
	SFTranslate translated = SFTranslate(&sphe, lux::Vector(0.0, 3.5, 0.0));

	Mask msk = Mask(&sphe);
	Mask msk2 = Mask(&ellp);
	Mask msk3 = Mask(&translated);
	//ColorField cf1 = ColorField(lux::Vector(0.0, 1.0, 0.0), &msk3);

	std::cout << translated.eval(lux::Vector());


	//ColorField cf = ColorField(lux::Vector(1.0, 0.0, 0.0), &msk2); 
	//FieldResult res = ColorField::eval(lux::Vector(0.0, 0.0, 80.0));
	
	//PrintVector("color", res.color);

	//lux::Vector color = rend.RayMarch(ray);
	//PrintVector("color after ray marching", color);
	
	Cutout ellipsesphere = Cutout(&ellp, &translated);
	Mask msk4 = Mask(&ellipsesphere);
	//ColorField cf = ColorField(lux::Vector(0.123, 0.5, 0.21), &msk4);
	//img.debugrgb();
	Mask msk5 = Mask(&tors);
	//ColorField cfTorus = ColorField(lux::Vector(0.0, 0.0, 1.0), &msk5);
	*/
	Renderer rend = Renderer(0.1075, 0.712);
	CameraSetup camset;
	camset.position = lux::Vector(10.0, 0.0, 0.0);
	camset.viewDirection = lux::Vector(0.0, 5.0, 0.0) - camset.position;
	camset.nearPlane = 14.0;
	camset.farPlane = 60.598; // 60.5
	camset.fov = 110.0;
	Image img (camset, 16*120, 9*120);


	/*camera turntable*/
	int nrOfImages = 120;
	double degreeInterval = 360.0 / (double) nrOfImages; // 
	double radius = 25.0;
	double currentAngle = 0.0;
	lux::Vector lookAt = lux::Vector(0.0, 4.98, 0.0);
	std::string imgName = "avatarnew";
	Avatar avatar;
	
	avatar.FinalAvatar();

	int beginFrom = 0;
	std::cout << "Image number to begin from (0-119): ";
	std::cin >> beginFrom;

	for (int i = beginFrom; i < nrOfImages; i++)
	{
		std::cout << "Generating image#" << i << "..." << std::endl;
		currentAngle = (i * degreeInterval); // mult i, final

		double z = radius * std::cos(DegToRad(currentAngle));
		double x = radius * std::sin(DegToRad(currentAngle));
		camset.position = lux::Vector(x, 0.0, z);
		camset.viewDirection = lookAt - camset.position;
		img.SetupCamera(camset);

		std::string filename = imgName + std::to_string(i);
		filename = "img/" + filename + ".exr";  
		//PrintVector("camera position", camset.position);
		img.CaptureImage(&rend);
		img.WriteImage(filename);

	}
	
	std::cout << "Done!" << std::endl;
	std::cin.get();
	return 0;
}