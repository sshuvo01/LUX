#pragma once
#include <iostream>
#include <strstream>
#include <string>
#include <fstream>
#include <vector>
#include "Vector.h"

struct Triangle
{
	lux::Vector p0;
	lux::Vector p1;
	lux::Vector p2;
};

class ModelLoader
{
public:
	ModelLoader(const std::string& filepath);
	~ModelLoader();
	
	std::vector<Triangle>		m_Triangles;
	void Debug();
private:
	std::string					m_Filepath;
	std::vector<lux::Vector>	m_Vertices;

	void LoadModel();
	std::vector<std::string> GetWords(const std::string& line) const;
};


