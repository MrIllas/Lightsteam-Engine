#pragma once

#include <string>
#include <vector>

class aiMesh;
class aiNode;
class aiScene;
class Mesh;
class GameObject;

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	void Start();
	void CleanUp();

public:
	static void ImportMesh(std::string filePath);
	static void LoadMeshFile( std::string filePath);

private:
	static Mesh GenerateMesh(aiMesh* mesh);
	static GameObject* GenerateGameObjects(aiNode* node, const aiScene* scene, GameObject* parent = nullptr);
};



