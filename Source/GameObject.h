#pragma once

#include "Globals.h"

#include "Component.h"

#include <vector>
#include <map>

#include <typeinfo>

class CompMeshRenderer;

class GameObject
{
public:
	GameObject(std::string name = "Spatial Node", bool spatial = true);
	~GameObject();

	void Update();


	Component* CreateComponent(CO_TYPE type);
	void DeleteComponent(CO_TYPE type);
	Component* GetComponent(CO_TYPE type);
	Component* GetComponentInChildren(CO_TYPE type);

	template <class T>
	T* GetComponent(CO_TYPE type)
	{ 
		return (T*)components[type];
	}

	void AddChildren(GameObject* go);

	bool HasChildren() { return (children.size() != 0) ? true : false; }

public:
	std::string name;
	bool selected;

	std::map<CO_TYPE, Component*> components;

	std::vector<GameObject*> children;
	GameObject* parent = nullptr;
};

