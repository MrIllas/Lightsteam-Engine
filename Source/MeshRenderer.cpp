#include "MeshRenderer.h"
#include "TextureImporter.h"

#include "MeshImporter.h"

#include "Shader.h"

#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"


MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Meshe meshData)
{
	this->mesh = meshData;

	//init
	VAO = 0;
	VBO = 0;
	EBO	 = 0;
	
	//Generation of buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

	//VERTEX POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//VERTEX NORMALS
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));


	//VERTEX TEXTURE COORDS
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoords));

	//VERTEX TANGENTS
	/*glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat)));*/

	/*glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);*/

	//Index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

	textureID = TextureImporter::CheckerImage();

	//Cleaning
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


MeshRenderer::~MeshRenderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	VAO = 0;
	VBO = 0;
	EBO = 0;
}

void MeshRenderer::Draw(Shader* shader)
{
	if (this->shader == nullptr) this->shader = shader;

	if (EBO != 0)
	{
		this->shader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader->SetInt("texture_albedo", 0);
		//

		//glActiveTexture(GL_TEXTURE0);

		float4x4 identity;
		identity = identity.identity; 


		this->shader->SetMat4("projection", CameraProperties::Instance()->editorCamera.GetProjectionMatrix());
		this->shader->SetMat4("view", CameraProperties::Instance()->editorCamera.GetViewMatrix());
		this->shader->SetMat4("model", &identity.v[0][0]);

		//Draw Mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
}

#pragma region Getters and Setters
void MeshRenderer::SetShader(Shader* shader)
{
	this->shader = shader;
}

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