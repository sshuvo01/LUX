#include "ModelLoader.h"
#define DEBUGVEC2(x)  x.X() << ", " << x.Y() << ", " << x.Z() 

ModelLoader::ModelLoader(const std::string& filepath)
	:m_Filepath(filepath)
{
	LoadModel();
}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::Debug()
{
	int count = 1;
	for (Triangle tr: m_Triangles)
	{
		std::cout << "Triangle #" << count << std::endl; 
		count++;
		std::cout << "p0: " << DEBUGVEC2(tr.p0) << std::endl;
		std::cout << "p1: " << DEBUGVEC2(tr.p1) << std::endl;
		std::cout << "p2: " << DEBUGVEC2(tr.p2) << std::endl << std::endl;
	}
}

void ModelLoader::LoadModel()
{
	std::ifstream stream(m_Filepath);
	std::string line;
	std::vector<std::string> wordVec;
	if (!stream.is_open())
	{
		std::cout << "could not open: " << m_Filepath << std::endl;
		return;
	}
	while (std::getline(stream, line))
	{
		if (line.size() != 0 && line[0] == '#') continue;

		if (line.find("v") != std::string::npos)
		{
			if (line[0] != 'v' && line[1] != ' ') continue;
			// a vertex
			//std::cout << line << std::endl;
			wordVec = GetWords(line);
			
			double x = std::stod(wordVec[1]);
			double y = std::stod(wordVec[2]);
			double z = std::stod(wordVec[3]);

			m_Vertices.push_back(lux::Vector(x, y, z));
		}
		else if (line.find("f") != std::string::npos)
		{
			if (line[0] != 'f' && line[1] != ' ') continue;
			wordVec = GetWords(line);
			int i = std::stoi(wordVec[1]);
			int j = std::stoi(wordVec[2]);
			int k = std::stoi(wordVec[3]);
			//std::cout << i << ", " << j << ", " << k << std::endl;
			m_Triangles.push_back({m_Vertices[i-1], m_Vertices[j-1], m_Vertices[k-1]});
		}
	}

	stream.close();
}

std::vector<std::string> ModelLoader::GetWords(const std::string & line) const
{
	std::string word = "";
	std::vector<std::string> wordVec;

	for (char ch : line)
	{
		if (ch == ' ' && word != "")
		{
			wordVec.push_back(word);
			word = "";
			continue;
		}
		if(ch != ' ') word += ch;
	}
	if(word != "") wordVec.push_back(word);

	return wordVec;
}

/*
ShaderProgramSource Shader::ParseShader(const std::string & filepath)
{
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

std::string Shader::ReadShader(const std::string & filepath)
{
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream theShader;

	while (getline(stream, line))
	{
		theShader << line << '\n';
	}

	return theShader.str();
}
*/