#pragma once

#include "Primitive.h"
#include "Glew/include/glew.h"
class MeshRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();

	void Draw();
	void Update();

private:
	void PrepareCubeBuffer();
	void PrepareSphereBuffer();

private:
	GLuint vertexBuffer;
	GLuint indexBuffer;
	Primitive meshData;
};

