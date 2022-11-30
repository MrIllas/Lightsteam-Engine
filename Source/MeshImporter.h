#pragma once

#include <string>

class aiMesh;
class aiNode;
class aiScene;
class Mesh;
class GameObject;
class ResourceMesh;

struct Meshe;

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	void Init();
	void CleanUp();

public:
	static void ImportToLibrary(ResourceMesh* resource);

	static GameObject* ImportMesh(std::string filePath, GameObject* parent = nullptr, bool dragAndDrop = false);
	
	static Meshe LoadMesh(std::string filePath);
private:
	static Meshe GenerateMesh(aiMesh* mesh);
	static GameObject* GenerateGameObjects(aiNode* node, const aiScene* scene, GameObject* parent = nullptr);
	static void SaveMesh(Meshe mesh, std::string filePath);
	
};



