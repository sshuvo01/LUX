#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#include "Image.h"
#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"
#include <iostream>

Image::Image(const CameraSetup & camset, unsigned int width, unsigned int height, unsigned int nrChannel)
	: m_Camera(), m_Width(width), m_Height(height), m_NrChannel(nrChannel)
{
	/*camera setup*/
	SetupCamera(camset);
	/**/
	m_RGB = new float[m_Width*m_Height*m_NrChannel](); // initialized to 0
	// add more lines
}

Image::~Image()
{
	delete[] m_RGB;
}

bool Image::WriteImage(const std::string & filepath) const
{
	float* rgb = m_RGB;
	int width = m_Width;
	int height = m_Height;
	const char* outfilename = filepath.c_str();

	EXRHeader header;
	InitEXRHeader(&header);

	EXRImage image;
	InitEXRImage(&image);

	image.num_channels = 3;

	std::vector<float> images[3];
	images[0].resize(width * height);
	images[1].resize(width * height);
	images[2].resize(width * height);

	// Split RGBRGBRGB... into R, G and B layer
	for (int i = 0; i < width * height; i++) {
		//std::cout << "br" << std::endl;
		//std::cout << rgb[3 * i + 0] << std::endl;
		//std::cout << rgb[3 * i + 1] << std::endl;
		//std::cout << rgb[3 * i + 2] << std::endl;

		images[0][i] = rgb[3 * i + 0]; // 255.0f;
		images[1][i] = rgb[3 * i + 1]; // 255.0f;
		images[2][i] = rgb[3 * i + 2]; // 255.0f;

	}

	float* image_ptr[3];
	image_ptr[0] = &(images[2].at(0)); // B
	image_ptr[1] = &(images[1].at(0)); // G
	image_ptr[2] = &(images[0].at(0)); // R

	image.images = (unsigned char**)image_ptr;
	image.width = width;
	image.height = height;

	header.num_channels = 3;
	header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
	// Must be (A)BGR order, since most of EXR viewers expect this channel order.
	strncpy(header.channels[0].name, "B", 255);
	header.channels[0].name[strlen("B")] = '\0';
	strncpy(header.channels[1].name, "G", 255);
	header.channels[1].name[strlen("G")] = '\0';
	strncpy(header.channels[2].name, "R", 255);
	header.channels[2].name[strlen("R")] = '\0';

	header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
	header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
	for (int i = 0; i < header.num_channels; i++) {
		header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
		header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of output image to be stored in .EXR
	}

	const char* err = NULL; // or nullptr in C++11 or later.
	int ret = SaveEXRImageToFile(&image, &header, outfilename, &err);
	if (ret != TINYEXR_SUCCESS) {
		fprintf(stderr, "Save EXR err: %s\n", err);
		FreeEXRErrorMessage(err); // free's buffer for an error message
		std::cout << "could not save" << std::endl;
		return ret;
	}
	printf("Saved exr file. [ %s ] \n", outfilename);

	//free((void*)rgb);

	free(header.channels);
	free(header.pixel_types);
	free(header.requested_pixel_types);
	return true;
}

void Image::CaptureImage(Renderer * renderer)
{
	lux::Vector color = lux::Vector();
	Ray theRay;
	lux::Vector rayDirection = lux::Vector();
	lux::Vector eye = m_Camera.eye();
	double sNear = m_Camera.nearPlane();
	double sFar = m_Camera.farPlane();

	for (unsigned int i = 0; i < m_Width; i++)
	{
		for (unsigned int j = 0; j < m_Height; j++)
		{
			rayDirection = m_Camera.view((double)i / (double)m_Width, 
				(double)j / (double)m_Height);
			theRay = { rayDirection, eye, sNear, sFar };
			color = renderer->RayMarch(theRay);
			//color = renderer->RayMarchNoEmission(theRay);
			/*save the color in the dynamic array*/
			SaveRBGValue(i, j, color);
			/*
			std::cout << i << ", " << j  << ", color: " << color[0]
				<< ", " << color[1] << ", " << color[2] << ", " << std::endl;
			std::cout << "ray direction: " << rayDirection[0] << ", " << rayDirection[1] << ", "
				<< rayDirection[2] << std::endl;

			std::cout << "camera position: " << eye[0] << ", " << eye[1] << ", " << eye[2] << std::endl;
			std::cout << "sNear: " << sNear << std::endl;
			std::cout << "sFar: " << sFar << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			*/
		}
	}
}

void Image::SetupCamera(const CameraSetup & camset)
{
	m_Camera.setEyeViewUp(camset.position, camset.viewDirection, camset.up);
	m_Camera.setFov(camset.fov);
	m_Camera.setAspectRatio(camset.aspectRatio);
	m_Camera.setNearPlane(camset.nearPlane);
	m_Camera.setFarPlane(camset.farPlane);
}


void Image::SaveRBGValue(unsigned int i, unsigned int j, const lux::Vector& rgbVal)
{
	unsigned int index = m_Width*j + i;
	for (unsigned int ind = 0; ind < m_NrChannel; ind++)
	{
		m_RGB[index*m_NrChannel + ind] = rgbVal[ind];
	}
}

void Image::debugrgb()
{
	for (int i = 0; i < m_Width*m_Height*m_NrChannel; i++)
	{
		std::cout << m_RGB[i] << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl;
}
