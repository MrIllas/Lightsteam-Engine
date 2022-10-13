#include "MeshRenderer.h"

#include "MeshImporter.h"

MeshRenderer::MeshRenderer()
{
	indexBuffer = 0;
	vertexBuffer = 0;
}

MeshRenderer::MeshRenderer(Mesh meshData)
{
	//this->meshData = MeshImporter::LoadMeshFile("../Output/Assets/BakerHouse.fbx");
	this->meshData = meshData;

	//Vertex Buffer creation
	//Creates a new Buffer that is sent to the VRam
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * meshData.GetVertex().size(), &meshData.GetVertex()[0], GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY); //Type of data
	glVertexPointer(3, GL_FLOAT, 0, NULL); //Use bind buffer as vertices

	//Index Buffer creation
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * meshData.GetIndex().size(), &meshData.GetIndex()[0], GL_STATIC_DRAW);
}


MeshRenderer::~MeshRenderer()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

void MeshRenderer::Draw()
{
	if (indexBuffer != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_TRIANGLES, meshData.GetIndex().size(), GL_UNSIGNED_INT, NULL);
	}
}

#pragma region Getters and Setters
float3 MeshRenderer::GetPosition()
{
	float3 toReturn = { 0 , 0 ,0 };

	return toReturn;
}

float3 MeshRenderer::GetRotation()
{
	float3 toReturn = { 0 , 0 ,0 };

	return toReturn;
}

float3 MeshRenderer::GetSize()
{
	float3 toReturn = { 0 , 0 ,0 };

	return toReturn;
}

void MeshRenderer::SetPosition(float3 newPos)
{

}

void MeshRenderer::SetRotation(float3 newRot)
{

}

void MeshRenderer::SetSize(float3 newSize)
{

}
#pragma endregion Getters and Setters