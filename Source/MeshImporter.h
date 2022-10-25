#pragma once

#include <string>
#include <vector>

class aiMesh;
class aiNode;
class aiScene;
class Mesh;
class GameObject;

struct Meshe;

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	void Start();
	void CleanUp();

public:
	static void ImportMesh(std::string filePath, GameObject* parent = nullptr, bool dragAndDrop = false);
	static void LoadMeshFile(std::string filePath);

private:
	static Meshe GenerateMesh(aiMesh* mesh);
	static GameObject* GenerateGameObjects(aiNode* node, const aiScene* scene, GameObject* parent = nullptr);
};



