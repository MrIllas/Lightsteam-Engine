#pragma once

//#include "Mesh.h"
#include "Glew/include/glew.h"

#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"

#include <vector>

#define VERTEX 3

typedef unsigned int uint;

struct Vertex {
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

struct Texture {
	unsigned int id;
	std::string type;
};

struct Meshe
{
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
};

class MeshRenderer
{
public:
	MeshRenderer();
	MeshRenderer(Meshe meshData);
	~MeshRenderer();

	void Draw();

	float3 GetPosition();
	float3 GetRotation();
	float3 GetSize();

	void SetPosition(float3 newPos);
	void SetRotation(float3 newRot);
	void SetSize(float3 newSize);

private:
	float4x4 matrix;

	uint VAO;
	uint VBO;
	uint EBO;

	uint indexBuffer;
	Meshe mesh;
};

