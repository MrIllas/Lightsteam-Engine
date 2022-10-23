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
	virtual ~Component();

	virtual void Init();
	virtual void Update();

	virtual void UpdateGUI();

	void Enable();
	void Disable();

public:
	CO_TYPE type;
	bool active;
	
	GameObject* owner = nullptr;
};

