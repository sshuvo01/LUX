#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace lux
{

	Camera::Camera()
	{
		setEyeViewUp(Vector(0, 0, 1), Vector(0, 0, -1), Vector(0, 1, 0));
		setFov(60.0);
		setAspectRatio(16.0 / 9.0);
		setNearPlane(0.0);
		setFarPlane(1.0e6);
	}

	void Camera::setEyeViewUp(const Vector& eye, const Vector& view, const Vector& up)
	{
		position = eye;
		axis_view = view.unitvector();
		axis_up = (up - (up*axis_view) * axis_view).unitvector();
		axis_right = (axis_view^axis_up).unitvector();
	}

	// view direction of a pixel at the fractional position x,y.
	// Nominally 0 <= x <= 1 and 0 <= y <= 1 for the primary fov,
	// but the values can extend beyond that
	const Vector Camera::view(const double x, const double y)
	{
		double xx = (2.0*x - 1.0)*htanfov;
		double yy = (2.0*y - 1.0)*vtanfov;
		return (axis_up * yy + axis_right * xx + axis_view).unitvector();
	}

	void Camera::setFov(const double fov)
	{
		FOV = fov;
		htanfov = tan(FOV*0.5*M_PI / 180.0);
		vtanfov = htanfov / aspect_ratio;
	}

	void Camera::setAspectRatio(const double ar)
	{
		aspect_ratio = ar;
		vtanfov = htanfov / aspect_ratio;
	}

}

