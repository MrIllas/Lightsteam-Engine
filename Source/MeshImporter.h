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
	static void LoadMeshFile( std::string filePath);

private:
	static Mesh LoadMesh(aiMesh* mesh);
	static void LoadNode(aiNode* node);
};



