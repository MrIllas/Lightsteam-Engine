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

class Material;
class Shader;
class Camera;

enum Debug_Normals
{
	OFF = 0,
	VERTEX,
	FACE,
	BOTH
};

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
	AABB localAABB;
	std::string path;
};

struct TextureData;

class MeshRenderer
{
public:
	MeshRenderer();
	MeshRenderer(Meshe meshData, bool debug = true);
	~MeshRenderer();

	void LiteDraw(Material* shader, float4x4 model, Camera* camera);
	void FullDraw(Material* shader, Shader* debugShader, float4x4 model, Camera* camera, Debug_Normals normals = Debug_Normals::OFF);

	void DrawMesh(Material* shader, Camera* camera, float4x4 model = float4x4::identity);

	void DrawNormals(Shader* shader, Camera* camera, float4x4 model, Debug_Normals normals);
	void DrawBBox(Shader* shader, Camera* camera, float4x4 model);
	void DrawFrustumBox(Shader* shader, Camera* camera, float4x4 model);

	void CreateNormals(float magnitude = 0.25f);
	void CreateBoundingBox();
	void CreateRay(float3 a, float3 b);
	void CleanNormals();

	void SetShader(Shader* shader);
	void SetDebugShader(Shader* shader);

	void SetTexture(Texture texture);

	void SetPosition(float3 newPos);
	void SetRotation(float3 newRot);
	void SetSize(float3 newSize);

public:
	Meshe mesh;

	std::string modelUuid;
	std::string uuid;

	bool planDelete = false;

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

	float time;
};

