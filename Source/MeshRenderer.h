#pragma once

#include "TextureImporter.h"

//#include "Mesh.h"
#include "Glew/include/glew.h"

#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Geometry/AABB.h"

#include <vector>

typedef unsigned int uint;

class Shader;
class Camera;

struct Vertex {
	Vertex()
	{
		this->position = float3(0, 0, 0);
		this->normal = float3(0, 0, 0);
		this->texCoords = float2(0, 0);
	}
	Vertex(float3 position, float3 normal, float2 texCoord)
	{
		this->position = position;
		this->normal = normal;
		this->texCoords = texCoord;
	}

	float3 position;
	float3 normal;
	float2 texCoords;
};

struct Meshe
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	uint numFaces;
	AABB bBox;
	std::string path;
};

struct TextureData;

class MeshRenderer
{
public:
	MeshRenderer();
	MeshRenderer(Meshe meshData);
	~MeshRenderer();

	void Draw(Shader* shader, Camera* camera, Texture text, float4x4 model = float4x4::identity);

	void DrawNormals(Shader* shader, Camera* camera, float4x4 model, bool faceNormals = false);

	void CreateNormals(float magnitude = 0.25f);
	void CreateBoundingBox();
	void CleanNormals();

	void SetShader(Shader* shader);
	void SetDebugShader(Shader* shader);

	void SetTexture(Texture texture);

	void SetPosition(float3 newPos);
	void SetRotation(float3 newRot);
	void SetSize(float3 newSize);

public:
	Meshe mesh;

private:
	uint VAO;
	uint VBO;
	uint EBO;

	//Vertex normals
	uint VNVAO;
	uint VNVBO;
	std::vector<float3> vNormals;

	//Face normals
	uint FNVAO;
	uint FNVBO;
	std::vector<float3> fNormals;

	//Bounding box
	uint BBVAO;
	uint BBVBO;
	std::vector<float3> bBox;

	Shader* shader = nullptr;
	Shader* debugShader = nullptr;
};

