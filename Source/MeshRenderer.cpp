#include "MeshRenderer.h"



MeshRenderer::MeshRenderer()
{
	meshData.Init(CYLINDER);
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, meshData.GetIndex().size(), GL_UNSIGNED_INT, NULL);
}

void MeshRenderer::Update()
{

}