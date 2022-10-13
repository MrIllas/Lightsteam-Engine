#pragma once

#include "MathGeoLib/include/Math/float2.h"

#include <queue>

class FrameBuffer;
class CompMeshRenderer;

class Renderer
{
public:
	Renderer(){};
	Renderer(float2 size);
	~Renderer();

	void Init();
	void Start();
	void CleanUp();

	void Render();
	void Resize(float2 size);

	FrameBuffer* GetFrameBufffer() { return frameBuffer; }

	void QueueMesh(CompMeshRenderer* mesh);

public:
	float2 size = {0, 0};

private:
	void PreUpdate();
	void Update();
	void PostUpdate();

	//std::vector<MeshRenderer*> meshes;
	std::queue<CompMeshRenderer*> meshes;

private:
	FrameBuffer* frameBuffer = nullptr;
};

