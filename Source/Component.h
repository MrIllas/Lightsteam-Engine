#pragma once

#include <string> 

enum CO_TYPE
{
	NONE,
	TRANSFORM,
	MESH_RENDERER,
	MATERIAL
};

class GameObject;

class Component
{
public:
	Component(GameObject* owner);
	~Component();

	virtual void Init();
	virtual void Update();

	void Enable();
	void Disable();

public:
	CO_TYPE type;
	bool active;
	
	GameObject* owner = nullptr;
};

