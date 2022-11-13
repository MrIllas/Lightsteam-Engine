#pragma once

#include <string> 

enum CO_TYPE
{
	NONE,
	TRANSFORM,
	MESH_RENDERER,
	MATERIAL,
	CAMERA,
	LAST
};

inline const char* CompTypeToString(CO_TYPE type)
{
	switch (type)
	{
		case TRANSFORM:   return "Transform";
		case MESH_RENDERER:   return "Mesh Renderer";
		case MATERIAL: return "Material";
		case CAMERA: return "Camera";
	}
}

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
	bool deleteQueue;
	
	GameObject* owner = nullptr;
};

