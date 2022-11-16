#pragma once

#include "JsonUtils.h"

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
	Component(GameObject* owner, std::string uuid);
	virtual ~Component();

	virtual void Init();
	virtual void Update();

	virtual void UpdateGUI();

	void Enable();
	void Disable();

	nlohmann::ordered_json Save();
	virtual void Load(nlohmann::json data);

protected:
	virtual nlohmann::ordered_json SaveUnique(nlohmann::JsonData data);

public:
	std::string uuid;
	CO_TYPE type;
	bool active;
	bool deleteQueue;
	
	GameObject* owner = nullptr;
};

