#pragma once

#include "Globals.h"

#include "Component.h"

#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();


	Component* CreateComponent(CO_TYPE type);
	Component* GetComponent();
	Component* GetComponentInChildren();

public:
	std::string name;

	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

private:
	bool CheckComponents(CO_TYPE type);
};

