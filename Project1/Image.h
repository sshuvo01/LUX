#pragma once
#include "Core.h"
#include "Camera.h"
#include "Renderer.h"

struct CameraSetup
{
	lux::Vector viewDirection = lux::Vector(0.0, 0.0, -1.0);
	lux::Vector up = lux::Vector(0.0, 1.0, 0.0);
	lux::Vector position = lux::Vector(0.0, 0.0, 1.0);
	double		fov = 90.0f;
	double		aspectRatio = (16.0 / 9.0);
	double		nearPlane = 0.0;
	double		farPlane = 1.0e6;
};

class Image
{
private:
	lux::Camera		m_Camera;
	CameraSetup		m_CameraSettings;
	float*			m_RGB;
	unsigned int	m_Width, m_Height;
	unsigned int	m_NrChannel;
	// i along the width, j along the height
public:
	Image(const CameraSetup& camset,
		unsigned int width = 1920, unsigned int height = 1080, unsigned int nrChannel = 3);
	
	Image() = delete;
	Image(const Image& copy) = delete;
	Image(const Image&& copy) = delete;
	Image& operator=(const Image& other) = delete;
	Image& operator=(const Image&& other) = delete;

	~Image();
	
	bool WriteImage(const std::string& filepath) const;
	void CaptureImage(Renderer* renderer);
	void SetupCamera(const CameraSetup& camset);
	void debugrgb();
private:
	void SaveRBGValue(unsigned int i, unsigned int j, const lux::Vector& rgbVal);
};

