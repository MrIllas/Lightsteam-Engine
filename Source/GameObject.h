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
	std::vector<T*> GetComponentsInChildrens(CO_TYPE type)
	{
		std::vector<T*> toReturn;
		
		for (int i = 0; i < children.size(); ++i)
		{
			T* aux = children[i]->GetComponent<T>(type);
		
			if(aux != nullptr) toReturn.emplace_back(aux);
		}

		return toReturn;
	}


	template <class T>
	T* GetComponent(CO_TYPE type)
	{ 
		if (components.count(type)) return (T*)components[type];
		else nullptr;
	}

	void AddChildren(GameObject* go);
	void RemoveChildren(GameObject* go);
	void SetParent(GameObject* go);

	bool HasChildren() { return (children.size() != 0) ? true : false; }

public:
	std::string name;
	bool selected;

	std::map<CO_TYPE, Component*> components;

	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

private:
	//Operator
	bool operator==(GameObject& other) const
	{
		if (this == &other) return true; //This is the pointer for 
		else return false;
	}
};

