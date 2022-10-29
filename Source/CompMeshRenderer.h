#pragma once
#include "Component.h"

#include "MeshRenderer.h"


struct CameraProperties;

class Shader;

class CompMeshRenderer : public Component
{
public:
	CompMeshRenderer(GameObject* owner);
	~CompMeshRenderer() override;

	void Update() override;

	void UpdateGUI() override;

	void Render(Shader* shader, Shader* debugShader);

	MeshRenderer* GetMesh();
	void SetMesh(MeshRenderer* mesh);
	
private:
	MeshRenderer* mesh = nullptr;

	CameraProperties* camInstance = nullptr;

	bool displayNormals;
	bool faceNormals;

	float normalsMagnitude;
};

