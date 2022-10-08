#pragma once

#include <string>
#include <vector>
class aiMesh;
class Mesh;

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	void Start();
	void CleanUp();

public:
	static std::vector<Mesh> LoadMeshFile( std::string filePath);

private:
	static Mesh LoadMeshNode(aiMesh* mesh);
};



