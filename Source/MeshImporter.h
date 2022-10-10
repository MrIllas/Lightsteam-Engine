#pragma once

#include <string>
#include <vector>

class aiMesh;
class aiNode;
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
	static Mesh LoadMesh(aiMesh* mesh);
	static void LoadNode(aiNode* node);
};



