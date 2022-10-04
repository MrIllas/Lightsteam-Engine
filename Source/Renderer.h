#pragma once

#include "FrameBuffer.h"

#include "MathGeoLib/include/Math/float2.h"

#include "MeshRenderer.h"

class Renderer
{
public:
	Renderer(float2 size);
	~Renderer();

	void Init();
	void Start();
	void CleanUp();

	void Draw();
	void Resize(float2 size);

	FrameBuffer* GetFrameBufffer() { return frameBuffer; }

public:
	float2 size = {0, 0};
private:
	void PreUpdate();
	void Update();
	void PostUpdate();

	std::vector<MeshRenderer*> meshes;

private:
	FrameBuffer* frameBuffer = nullptr;
};

