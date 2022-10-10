#pragma once

#include "Mesh.h"
#include "Glew/include/glew.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"

#include <vector>

class MeshRenderer
{
public:
	MeshRenderer();
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

	std::vector<GLuint> vertexBuffer;
	std::vector<GLuint> indexBuffer;
	std::vector<Mesh> meshData;
};

