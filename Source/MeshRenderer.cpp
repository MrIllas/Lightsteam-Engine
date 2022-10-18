#include "MeshRenderer.h"

#include "MeshImporter.h"

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

	//Index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);
	

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
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat)));


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

void MeshRenderer::Draw()
{
	if (indexBuffer != 0)
	{

		//unsigned int diffuseNr = 1;
		//unsigned int specularNr = 1;
		//for (unsigned int i = 0; i < mesh.textures.size(); i++)
		//{
		//	glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		//	// retrieve texture number (the N in diffuse_textureN)
		//	std::string number;
		//	std::string name = mesh.textures[i].type;
		//	if (name == "texture_diffuse")
		//		number = std::to_string(diffuseNr++);
		//	else if (name == "texture_specular")
		//		number = std::to_string(specularNr++);

		//	//shader.setInt(("material." + name + number).c_str(), i);
		//	glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
		//}

		//Draw Mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
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