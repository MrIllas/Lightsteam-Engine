#include "MeshRenderer.h"
#include "TextureImporter.h"

#include "MeshImporter.h"

#include "Shader.h"
#include "Material.h"
#include "ShaderUniform.h"

#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Meshe meshData, bool debug)
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

	//Index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

	//Cleaning
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (debug)
	{
		CreateNormals();
		CreateBoundingBox();
	}
	
	time = 0;
}


MeshRenderer::~MeshRenderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	VAO = 0;
	VBO = 0;
	EBO = 0;

	CleanNormals();

	//BoundingBox
	glDeleteVertexArrays(1, &BBVAO);
	glDeleteBuffers(1, &BBVBO);

	BBVAO = 0;
	BBVBO = 0;
}

#pragma region Drawing
void MeshRenderer::LiteDraw(Material* material, float4x4 model, Camera* camera)
{
	if (this->shader == nullptr) this->shader = shader;

	DrawMesh(material, camera, model);
}

void MeshRenderer::FullDraw(Material* material, Shader* debugShader, float4x4 model, Camera* camera, Debug_Normals normals)
{
	if (this->shader == nullptr) this->shader = shader;
	if (this->debugShader == nullptr) this->debugShader = shader;

	DrawMesh(material, camera, model);

	//Debug
	debugShader->Use();
	debugShader->SetMat4("Projection", camera->GetProjectionMatrix());
	debugShader->SetMat4("View", camera->GetViewMatrix());
	debugShader->SetMat4("Model", &model.v[0][0]);

	if (normals != Debug_Normals::OFF) DrawNormals(debugShader, camera, model, normals);

	DrawBBox(debugShader, camera, model);
}

void MeshRenderer::DrawMesh(Material* material, Camera* camera, float4x4 model)
{
	if (material == nullptr || material->GetShader() == nullptr) return;
	if (EBO != 0)
	{
		Shader* shader = material->GetShader();
		shader->Use();

		//Must have uniforms
		shader->SetMat4("Projection", camera->GetProjectionMatrix());
		shader->SetMat4("View", camera->GetViewMatrix());
		shader->SetMat4("Model", &model.v[0][0]);

		for (int i = 0; i < material->uniforms.size(); ++i)
		{
			if (material->uniforms[i]->name == "Projection" || material->uniforms[i]->name == "View" || material->uniforms[i]->name == "Model") continue;
			material->uniforms[i]->Update(shader);
		}
		
		//Draw Mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
}

void MeshRenderer::DrawNormals(Shader* shader, Camera* camera, float4x4 model, Debug_Normals normals)
{
	if (normals == Debug_Normals::VERTEX || normals == Debug_Normals::BOTH)
	{
		glBindVertexArray(VNVAO);
		glDrawArrays(GL_LINES, 0, vNormals.size());
	}
	glBindVertexArray(0);
	if (normals == Debug_Normals::FACE || normals == Debug_Normals::BOTH)
	{
		glBindVertexArray(FNVAO);
		glDrawArrays(GL_LINES, 0, fNormals.size());
	}

	glBindVertexArray(0);
}

void MeshRenderer::DrawBBox(Shader* shader, Camera* camera, float4x4 model)
{
	glBindVertexArray(BBVAO);
	glDrawArrays(GL_LINES, 0, bBox.size());

	glBindVertexArray(0);
}

void MeshRenderer::DrawFrustumBox(Shader* shader, Camera* camera, float4x4 model)
{
	if (this->debugShader == nullptr) this->debugShader = shader;

	debugShader->Use();
	debugShader->SetMat4("Projection", camera->GetProjectionMatrix());
	debugShader->SetMat4("View", camera->GetViewMatrix());
	debugShader->SetMat4("Model", &model.v[0][0]);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, mesh.indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

#pragma endregion Drawing methods

void MeshRenderer::CreateNormals(float magnitude)
{
	//VERETX NORMALS
	vNormals.reserve(mesh.vertices.size() * 2);

	VNVAO = 0;
	VNVBO = 0;

	for (int i = 0; i < mesh.vertices.size(); ++i)
	{
		vNormals.emplace_back(mesh.vertices[i].position);
		vNormals.emplace_back(mesh.vertices[i].position + mesh.vertices[i].normal * magnitude);
	}

	//Buffer generation
	glGenVertexArrays(1, &VNVAO);
	glGenBuffers(1, &VNVBO);

	glBindVertexArray(VNVAO);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, VNVBO);
	glBufferData(GL_ARRAY_BUFFER, vNormals.size() * sizeof(float3), &vNormals[0], GL_STATIC_DRAW);

	//Vertex Postition
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);


	///FACE NORMALS
	fNormals.reserve((mesh.indices.size() / 3) * 2);

	FNVAO = 0;
	FNVBO = 0;

	for (int i = 0; i < mesh.indices.size(); i += 3)
	{
		int aux[3] =
		{
			mesh.indices[i],
			mesh.indices[i+1],
			mesh.indices[i+2]
		};

		float3 face = (mesh.vertices[aux[0]].position + mesh.vertices[aux[1]].position + mesh.vertices[aux[2]].position) / 3;
		float3 dir = (mesh.vertices[aux[0]].normal + mesh.vertices[aux[1]].normal + mesh.vertices[aux[2]].normal) / 3;
		dir.Normalize();

		fNormals.emplace_back(face);
		fNormals.emplace_back(face + dir * magnitude);
	}

	//Buffer generation
	glGenVertexArrays(1, &FNVAO);
	glGenBuffers(1, &FNVBO);
	
	glBindVertexArray(FNVAO);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, FNVBO);
	glBufferData(GL_ARRAY_BUFFER, fNormals.size() * sizeof(float3), &fNormals[0], GL_STATIC_DRAW);

	//Vertex Postition
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

void MeshRenderer::CreateBoundingBox()
{
	int i[24] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5,
		2, 6, 3, 7,
		3, 1, 0, 2,
		6, 4, 7, 5
	};

	float3 auxArr[8];
	mesh.localAABB.GetCornerPoints(auxArr);
	bBox.reserve(8);

	BBVAO = 0;
	BBVBO = 0;

	for (int ii : i)
	{
		bBox.emplace_back(auxArr[ii]);
	}

	//Buffer
	glGenVertexArrays(1, &BBVAO);
	glGenBuffers(1, &BBVBO);

	glBindVertexArray(BBVAO);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, BBVBO);
	glBufferData(GL_ARRAY_BUFFER, bBox.size() * sizeof(float3), &bBox[0], GL_STATIC_DRAW);

	//Vertex Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

void MeshRenderer::CreateRay(float3 a, float3 b)
{
	int i[2] = { 0, 1 };

	float3 auxArr[2] = { a , b };

	//Buffer
	glGenVertexArrays(1, &BBVAO);
	glGenBuffers(1, &BBVBO);

	glBindVertexArray(BBVAO);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, BBVBO);
	glBufferData(GL_ARRAY_BUFFER, bBox.size() * sizeof(float3), &auxArr[0], GL_STATIC_DRAW);

	//Vertex Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

void MeshRenderer::CleanNormals()
{
	vNormals.clear();
	
	glDeleteVertexArrays(1, &VNVAO);
	glDeleteBuffers(1, &VNVBO);

	VNVAO = 0;
	VNVBO = 0;

	fNormals.clear();

	glDeleteVertexArrays(1, &FNVAO);
	glDeleteBuffers(1, &FNVBO);

	FNVAO = 0;
	FNVBO = 0;
}

#pragma region Getters and Setters
void MeshRenderer::SetShader(Shader* shader)
{
	this->shader = shader;
}

void MeshRenderer::SetDebugShader(Shader* shader)
{
	this->debugShader = shader;
}
#pragma endregion Getters and Setters