#pragma once

#include "Globals.h"

#include "Component.h"

#include <vector>
#include <map>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();


	Component* CreateComponent(CO_TYPE type);
	void DeleteComponent(CO_TYPE type);
	Component* GetComponent(CO_TYPE type);
	Component* GetComponentInChildren(CO_TYPE type);

public:
	std::string name;

	std::map<CO_TYPE, Component*> components;

	std::vector<GameObject*> children;
	GameObject* parent = nullptr;
};

