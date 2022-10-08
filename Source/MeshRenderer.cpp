#include "MeshRenderer.h"

#include "MeshImporter.h"

MeshRenderer::MeshRenderer()
{
	meshData = MeshImporter::LoadMeshFile("../Output/Assets/BakerHouse.fbx");

	vertexBuffer.resize(meshData.size());
	indexBuffer.resize(meshData.size());

	//Vertex Buffer creation
	//Creates a new Buffer that is sent to the VRam
	for(uint i = 0; i < meshData.size(); ++i)
	{
		glGenBuffers(1, &vertexBuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * meshData[i].GetVertex().size(), &meshData[i].GetVertex()[0], GL_STATIC_DRAW);

		glEnableClientState(GL_VERTEX_ARRAY); //Type of data
		glVertexPointer(3, GL_FLOAT, 0, NULL); //Use bind buffer as vertices

		//Index Buffer creation
		glGenBuffers(1, &indexBuffer[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * meshData[i].GetIndex().size(), &meshData[i].GetIndex()[0], GL_STATIC_DRAW);
	}
}

MeshRenderer::~MeshRenderer()
{
	for (uint i = 0; i < meshData.size(); ++i)
	{
		glDeleteBuffers(1, &vertexBuffer[i]);
		glDeleteBuffers(1, &indexBuffer[i]);
	}
}

void MeshRenderer::Draw()
{
	for (uint i = 0; i < meshData.size(); ++i)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer[i]);
		glDrawElements(GL_TRIANGLES, meshData[i].GetIndex().size(), GL_UNSIGNED_INT, NULL);
	}
}

void MeshRenderer::Update()
{

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