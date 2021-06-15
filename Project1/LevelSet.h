#ifndef LEVELSET_H
#define LEVELSET_H
#pragma once
#include "ModelLoader.h"
#include "ScalarGrid.h"
#include "Volume.h"
#include <string>
#include <limits>
#include <algorithm>
#include <queue>

inline bool RayIntersectsTriangle(const lux::Vector& rayOrigin,
	const lux::Vector& rayVector,
	const lux::Vector& vertex0,
	const lux::Vector& vertex1,
	const lux::Vector& vertex2,
	lux::Vector& outIntersectionPoint)
{
	const double EPSILON = 0.00000000001;
	//Vector3D vertex0 = inTriangle->vertex0;
	//Vector3D vertex1 = inTriangle->vertex1;
	//Vector3D vertex2 = inTriangle->vertex2;



	lux::Vector edge1, edge2, h, s, q;
	double a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	//h = rayVector.crossProduct(edge2);
	h = rayVector ^ edge2;
	//a = edge1.dotProduct(h);
	a = edge1 * h;

	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.
	f = 1.0 / a;
	s = rayOrigin - vertex0;
	//u = f * s.dotProduct(h);
	u = (s * h);
	u *= f;

	if (u < 0.0 || u > 1.0)
		return false;
	//q = s.crossProduct(edge1);
	q = s ^ edge1;

	//v = f * rayVector.dotProduct(q);
	v = (rayVector * q);
	v *= f;

	if (v < 0.0 || u + v > 1.0)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	double t = (edge2 * q);
	t *= f;

	if (t > EPSILON) // ray intersection
	{
		outIntersectionPoint = rayOrigin + rayVector * t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
}

class LevelSet : public lux::Volume<double>
{
public:
	LevelSet(const std::string& filepath, GridDetail gridSetting);
	~LevelSet();

	const double eval(const lux::Vector& x) const;
	void ClearSubGrid(const lux::Vector& p1, const lux::Vector& p2, double clearVal);
	void StampSubGridInGrid(const lux::Vector& p1, const lux::Vector& p2, ScalarGrid* gridToWrite) const;
	void Debug() const;
private:
	ScalarGrid				m_Grid;
	double					m_GridClearVal;
	std::string				m_Filepath;
	ModelLoader				m_Loader;
	std::queue<GridPoint>	m_PointsInside;
	std::queue<GridPoint>	m_PointsOutside;

	/*private functions*/
	void LoadFromGeometry();
	void StoreTriangle(const Triangle& tr);
	const double ClosestDistance(const Triangle& tr, lux::Vector& x) const;
	const lux::Vector ProjectOnTriangle(const Triangle& tr, lux::Vector& xijk) const;
	bool PointIsOnTriangle(const Triangle& tr, const lux::Vector& p) const;
	const double DistanceFromEdge(const lux::Vector& e, const lux::Vector& p,
		const lux::Vector& x) const;
	bool InsideGeometry(const lux::Vector& ps, const lux::Vector& d);
	void GrowBoundary();
};

#endif