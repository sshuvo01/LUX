#include "LevelSet.h"
#include <iostream>
#define DEBUGVEC(x)  x.X() << ", " << x.Y() << ", " << x.Z() 
#define DEBUGGP(x) x.i << ", " << x.j << ", " << x.k 
#define WAIT std::cout << "waiting...\n"; std::cin.get(); std::cin.get()

LevelSet::LevelSet(const std::string& filepath, GridDetail gridSetting)
	:m_Grid(gridSetting, 0.0), // -920000.0
	m_GridClearVal(90000000.0),
	m_Filepath(filepath),
	m_Loader(m_Filepath)
{
	LoadFromGeometry();
}

LevelSet::~LevelSet()
{
}

const double LevelSet::eval(const lux::Vector & x) const
{
	double val = m_Grid.eval(x);
	//if (val > 0.0) std::cout << "level set val : " << val << ", at " << DEBUGVEC(x) << std::endl;
	return val;
}

void LevelSet::ClearSubGrid(const lux::Vector& p1, const lux::Vector& p2, double clearVal)
{
	GridPoint from = m_Grid.VectorToGridPoint(p1);
	GridPoint to = m_Grid.VectorToGridPoint(p2);

	std::cout << "Clearing from: ";
	std::cout << "Point: " << from.i << ", " << from.j << ", " << from.k << std::endl;

	std::cout << "Clearing to: ";
	std::cout << "Point: " << to.i << ", " << to.j << ", " << to.k << std::endl;



	for (int i = from.i; i < to.i; i++)
	{
		for (int j = from.j; j < to.j; j++)
		{
			for (int k = from.k; k < to.k; k++)
			{
				m_Grid.SetGridData({ i, j, k }, clearVal);
			}
		}
	}

	/*
	lux::Vector delta = m_Grid.GetSetting().delta;
	for (double x = p1.X(); x <= p2.X(); x += delta.X())
	{
		for (double y = p1.Y(); y <= p2.Y(); y += delta.Y())
		{
			for (double z = p1.Z(); z <= p2.Z(); z += delta.Z())
			{
				lux::Vector thisVec(x, y, z);
				GridPoint thisPoint = m_Grid.VectorToGridPoint(thisVec);
				m_Grid.SetGridData(thisPoint, clearVal);
			}
		}
	}
	*/
	//std::cin.get();
	//std::cin.get();
}

void LevelSet::StampSubGridInGrid(const lux::Vector & p1, const lux::Vector & p2, 
	ScalarGrid * gridToWrite) const
{
	GridPoint from = m_Grid.VectorToGridPoint(p1);
	GridPoint to = m_Grid.VectorToGridPoint(p2);

	GridDetail setting = gridToWrite->GetSetting();
	for (int i = 0; i < setting.sizeN.i; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			for (int k = 0; k < setting.sizeN.k; k++)
			{
				lux::Vector thisVec = gridToWrite->GridPointToVector({ i, j, k });
				GridPoint levelSetPoint = m_Grid.VectorToGridPoint(thisVec);

				if (levelSetPoint.i < from.i || levelSetPoint.i > to.i) continue;
				if (levelSetPoint.j < from.j || levelSetPoint.j > to.j) continue;
				if (levelSetPoint.k < from.k || levelSetPoint.k > to.k) continue;
				// write this value to the given grid
				double valueToWrite = m_Grid.GetGridData(levelSetPoint);
				//std::cout << "value to write: " << valueToWrite << std::endl;
				gridToWrite->SetGridData({ i, j, k }, valueToWrite);
			}
		}
	}

}

void LevelSet::Debug() const
{
	GridDetail setting = m_Grid.GetSetting();
	for (int i = 0; i < setting.sizeN.i; i++)
	{
		for (int j = 0; j < setting.sizeN.j; j++)
		{
			for (int k = 0; k < setting.sizeN.k; k++)
			{
				GridPoint point = { i, j, k };
				std::cout << "{i, j, k} " << DEBUGGP(point);
				std::cout << ": " << m_Grid.GetGridData(point) << std::endl;
				if (m_Grid.GetGridData(point) > 1000.0)
				{
					std::cout << "WHY?\n";
					WAIT;
				}
			}
		}
	}
}

void LevelSet::LoadFromGeometry()
{
	if (m_Loader.m_Triangles.size() == 0)
	{
		std::cout << "Could not load model " << m_Filepath << std::endl;
		return;
	}
	else
	{
		std::cout << "Loaded " << m_Filepath << std::endl;
	}

	m_Grid.Clear(m_GridClearVal);
	
	for (Triangle tr : m_Loader.m_Triangles)
	{
		//lux::Vector p0 = tr.p0;// (v1.X, v1.Y, v1.Z);
		//lux::Vector p1 = tr.p1;//lv2(v2.X, v2.Y, v2.Z);
		//lux::Vector p2 = tr.p2;//(v3.X, v3.Y, v3.Z);

		//std::cout << "p0: " << DEBUGVEC(p0) << std::endl;
		//std::cout << "p1: " << DEBUGVEC(p1) << std::endl;
		//std::cout << "p2: " << DEBUGVEC(p2) << std::endl << std::endl;

		StoreTriangle(tr);
	}
	

	std::cout << "Triangles...\n";
	/*ray intersection to check if inside or outside*/
	GridDetail gridSet = m_Grid.GetSetting();
	for (int i = 0; i < gridSet.sizeN.i; i++)
	{
		for (int j = 0; j < gridSet.sizeN.j; j++)
		{
			for (int k = 0; k < gridSet.sizeN.k; k++)
			{
				GridPoint point = {i, j, k};

				const double EPSILON = 0.00000000001;
				if (m_Grid.GetGridData(point) == m_GridClearVal)
				{
					/*grid point not updated*/
					continue;
				}

				lux::Vector ps = m_Grid.GridPointToVector(point);
				lux::Vector d(1.230, 223.30, 11.3230); // direction
				d.normalize();
				if (InsideGeometry(ps, d))
				{
					//std::cout << "this point is inside: " << DEBUGVEC(ps) << std::endl;
					//WAIT;
					m_PointsInside.push(point);
				}
				else
				{
					//std::cout << "this point is outside: " << DEBUGVEC(ps) << std::endl;
					//WAIT;
					m_Grid.SetGridData(point, -m_Grid.GetGridData(point));
					m_PointsOutside.push(point);
				}

			} // end of for
		}
	} // end of for [i, j, k]
	//Debug();
	//return;
	std::cout << "Ray intersection\n";
	/*growth process*/
	GrowBoundary();

	//Debug();
	std::cout << "Growth process...\n";
} 

void LevelSet::StoreTriangle(const Triangle& tr)
{
	GridPoint bBox0, bBox1;
	double minX = std::min(std::min(tr.p0.X(), tr.p1.X()), tr.p2.X());
	double minY = std::min(std::min(tr.p0.Y(), tr.p1.Y()), tr.p2.Y());
	double minZ = std::min(std::min(tr.p0.Z(), tr.p1.Z()), tr.p2.Z());

	double maxX = std::max(std::max(tr.p0.X(), tr.p1.X()), tr.p2.X());
	double maxY = std::max(std::max(tr.p0.Y(), tr.p1.Y()), tr.p2.Y());
	double maxZ = std::max(std::max(tr.p0.Z(), tr.p1.Z()), tr.p2.Z());

	bBox0 = m_Grid.VectorToGridPoint(lux::Vector(minX, minY, minZ));
	bBox1 = m_Grid.VectorToGridPoint(lux::Vector(maxX, maxY, maxZ));

	bBox1.i++;
	bBox1.j++;
	bBox1.k++;

	
	//std::cout << "bbox0: " <<  DEBUGGP(bBox0) << std::endl;
	//std::cout << "bbox1: " << DEBUGGP(bBox1) << std::endl;
	//WAIT;
	
	// iterate inside the bounding box
	for (int i = bBox0.i; i <= bBox1.i; i++)
	{
		for (int j = bBox0.j; j <= bBox1.j; j++)
		{
			for (int k = bBox0.k; k <= bBox1.k; k++)
			{
				GridPoint point = { i, j, k };

				lux::Vector x = m_Grid.GridPointToVector(point);
				double cd = ClosestDistance(tr, x);
				double gridVal = m_Grid.GetGridData(point);
				m_Grid.SetGridData(point, std::min(cd, gridVal));
				//m_Grid.SetGridData({ i, j, k }, 0.1);
			}
		}
	}
} // end of StoreTriangle

const double LevelSet::ClosestDistance(const Triangle& tr, lux::Vector& x) const
{
	lux::Vector pt = ProjectOnTriangle(tr, x);
	/*pt is projected on the triangle, maybe outside of the triangle*/
	lux::Vector e1 = tr.p1 - tr.p0;
	lux::Vector e2 = tr.p2 - tr.p0;
	lux::Vector e3 = tr.p2 - tr.p1;
	lux::Vector n = e1 ^ e2;
	n.normalize();

	if (PointIsOnTriangle(tr, pt))
	{
		lux::Vector xp0 = x - tr.p0;
		return std::abs(xp0*n);
	}
	else
	{
		double d01, d12, d02;
		d01 = DistanceFromEdge(e1, tr.p0, x);
		d12 = DistanceFromEdge(e2, tr.p0, x);
		d02 = DistanceFromEdge(e3, tr.p1, x);
		return std::min(d01, std::min(d12, d02));
	}
} // end of ClosestDistance

const lux::Vector LevelSet::ProjectOnTriangle(const Triangle& tr, lux::Vector& xijk) const
{
	lux::Vector e1 = tr.p1 - tr.p0;
	lux::Vector e2 = tr.p2 - tr.p0;
	lux::Vector n = e1 ^ e2;
	n.normalize();

	double d = n * (tr.p0 - xijk);
	return xijk + d * n;
}

bool LevelSet::PointIsOnTriangle(const Triangle& tr, const lux::Vector& p) const
{
	lux::Vector e1 = tr.p1 - tr.p0;
	lux::Vector e2 = tr.p2 - tr.p0;
	lux::Vector n = e2 ^ e1;
	n.normalize();

	lux::Vector pp0 = p - tr.p0;
	lux::Vector e2e1 = e2 ^ e1;
	double e2e1Mag = e2e1.magnitude();
	double u = ((e2 ^ pp0) * e2e1) / (e2e1Mag * e2e1Mag);

	lux::Vector e1e2 = e1 ^ e2;
	double e1e2Mag = e1e2.magnitude();
	double v = ((e1 ^ pp0) * e1e2) / (e2e1Mag * e2e1Mag);

	double uv = u + v;

	//std::cout << "p: " << DEBUGVEC(p) << std::endl;
	//std::cout << "u, v: " << u << ", " << v << std::endl;

	bool cond1 = 0.0 <= u && u <= 1.0;
	bool cond2 = 0.0 <= v && v <= 1.0;
	bool cond3 = 0.0 <= uv && uv <= 1.0;

	return cond1 && cond2 && cond3;
}

const double LevelSet::DistanceFromEdge(const lux::Vector& e, const lux::Vector& p,
	const lux::Vector& x) const
{
	lux::Vector px = p - x;
	double eMag = e.magnitude();
	double u = (-e * px) / (eMag * eMag);

	if (u < 0) u = 0.0;
	else if (u > 1.0) u = 1.0;

	double vMag = (p - x + u * e).magnitude();
	return vMag * vMag;
}


bool LevelSet::InsideGeometry(const lux::Vector& ps, const lux::Vector& d)
{
	int intersectCount = 0;

	for (Triangle tr : m_Loader.m_Triangles)
	{
		//lux::Vector p0 = tr.p0;// (v1.X, v1.Y, v1.Z);
		//lux::Vector p1 = tr.p1;// (v2.X, v2.Y, v2.Z);
		//lux::Vector p2 = tr.p2;// (v3.X, v3.Y, v3.Z);

		lux::Vector e1 = tr.p1 - tr.p0;
		lux::Vector e2 = tr.p2 - tr.p0;
		lux::Vector n = e1 ^ e2;
		n.normalize();

		lux::Vector rayint;
		
		//if (RayIntersectsTriangle(ps, d, p0, p1, p2, rayint))
		//{
			//std::cout << p.X() << ", " << p.Y() << ", " << p.Z() << std::endl;
			/*
			std::cout << "ps: " << DEBUGVEC(ps) << std::endl;
			std::cout << "d: " << DEBUGVEC(d) << std::endl;
			std::cout << "int point: " << DEBUGVEC(rayint) << std::endl;
			//std::cout << "t: " << t << std::endl << std::endl;
			std::cout << "hits tirngle: \n";
			std::cout << "p0: " << DEBUGVEC(p0) << std::endl;
			std::cout << "p1: " << DEBUGVEC(p1) << std::endl;
			std::cout << "p2: " << DEBUGVEC(p2) << std::endl << std::endl;
			*/

			//intersectCount++;
		//}
		/*intersection*/
		
		lux::Vector e1e2 = e1 ^ e2;
		double t = ((tr.p0 - ps) * e1e2) / (d * e1e2);
		if (t < 0)
		{
			// doesn't hit this triangle
			continue;
		}
		else if(t > 0)
		{
			lux::Vector p = ps + (t * d);
			//std::cout << p.X() << ", " << p.Y() << ", " << p.Z() << std::endl;
			//std::cout << "ps: " << DEBUGVEC(ps) << std::endl;
			//std::cout << "d: " << DEBUGVEC(d) << std::endl;
			//std::cout << "t: " << t << std::endl << std::endl;

			//WAIT;
			if (PointIsOnTriangle(tr, p))
			{
				// hit this triangle
				/*
				std::cout << "p: " << DEBUGVEC(p) << std::endl;
				std::cout << "ps: " << DEBUGVEC(ps) << std::endl;
				std::cout << "d: " << DEBUGVEC(d) << std::endl;
				std::cout << "t: " << t << std::endl;
				std::cout << "hits tirngle: \n";
				std::cout << "p0: " << DEBUGVEC(p0) << std::endl;
				std::cout << "p1: " << DEBUGVEC(p1) << std::endl;
				std::cout << "p2: " << DEBUGVEC(p2) << std::endl << std::endl;
				*/
				intersectCount++;
			}
		}
	}

	//std::cout << intersectCount << std::endl;
	return intersectCount % 2;
}

void LevelSet::GrowBoundary()
{
	/*adjacency*/
	std::vector<GridPoint> adj;

	for (int adji = -1; adji <= 1; adji++)
	{
		for (int adjj = -1; adjj <= 1; adjj++)
		{
			for (int adjk = -1; adjk <= 1; adjk++)
			{
				//std::cout << adji << ", " << adjj << ", " << adjk << std::endl;
				adj.push_back({ adji, adjj, adjk });
			}
		}
	}


	GridDetail setting = m_Grid.GetSetting();
	double avgDelta = (setting.delta.X() + setting.delta.Y() 
		+ setting.delta.Z()) / 3.0;
	
	/*grow inside*/
	std::cout << "# of points updated inside: " << m_PointsInside.size() << std::endl;
	std::cout << "# of points updated outside: " << m_PointsOutside.size() << std::endl;
	while (!m_PointsInside.empty())
	{
		GridPoint temp = m_PointsInside.front();
		m_PointsInside.pop();
		for (int iter = 0; iter < adj.size(); iter++)
		{
			GridPoint p = { temp.i + adj[iter].i, temp.j + adj[iter].j, temp.k + adj[iter].k };
			double gridVal = m_Grid.GetGridData(p);
			double gridVal00 = m_Grid.GetGridData(temp);

			if (gridVal == m_GridClearVal)
			{
				m_PointsInside.push(p); 
				m_Grid.SetGridData(p, gridVal00 + avgDelta);
			}
			//else std::cout << gridVal << std::endl;
		}
				//std::cout << m_PointsInside.size() << std::endl;
	}

	std::cout << "grown inside\n";
	while (!m_PointsOutside.empty())
	{
		GridPoint temp = m_PointsOutside.front();
		m_PointsOutside.pop();
		for (int iter = 0; iter < adj.size(); iter++)
		{
			GridPoint p = { temp.i + adj[iter].i, temp.j + adj[iter].j, temp.k + adj[iter].k };
			double gridVal = m_Grid.GetGridData(p);
			double gridVal00 = m_Grid.GetGridData(temp);
			if (gridVal == m_GridClearVal)
			{
				//std::cout << DEBUGGP(p) << std::endl;
				//std::cout << gridVal00 << std::endl;
				//WAIT;
				m_PointsOutside.push(p);
				m_Grid.SetGridData(p, gridVal00 - avgDelta);
			}
			//else std::cout << gridVal << std::endl;
		}
		//std::cout << m_PointsOutside.size() << std::endl;
		//std::cin.get();
	}

	std::cout << "grown outside\n";
	//Debug();
	/*grow outside*/
}
