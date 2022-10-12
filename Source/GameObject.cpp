#include "GameObject.h"

#include "CompTransform.h"
#include "CompMeshRenderer.h"

GameObject::GameObject(bool spatial)
{
	name = "Node";

	if(spatial) CreateComponent(CO_TYPE::TRANSFORM);
}

GameObject::~GameObject()
{
	//Delete Children
	for (int i = 0; i < children.size(); ++i)
	{
		if (children[i] != nullptr)
		{
			RELEASE(children[i]);
		}
	}
	children.clear();

	//Delete Component
	for (auto const& comp : components)
	{
		if (comp.second != nullptr)
		{
			RELEASE(components[comp.first]);
		}
	}
	components.clear();
}

void GameObject::Update()
{
	for (auto const& comp : components)
	{
		if (comp.second->active)
		{
			comp.second->Update();
		}
	}
}

Component* GameObject::CreateComponent(CO_TYPE type)
{
	if (GetComponent(type) != nullptr)
	{
		LOG("A component of type %s already exists in %s !", std::to_string(type).c_str(), name.c_str());
		return nullptr; 
	}

	Component* toReturn = nullptr;

	switch (type)
	{
		case TRANSFORM:
			toReturn = new CompTransform(this);
			break;
		case MESH_RENDERER:
			toReturn = new CompMeshRenderer(this);
			break;
		case MATERIAL:

			break;
	}

	if (toReturn != nullptr) components[type] = toReturn;

	return toReturn;
}

void GameObject::DeleteComponent(CO_TYPE type)
{
	Component* comp = components[type];

	if (comp != nullptr)
	{
		RELEASE(comp);
	}
	components.erase(type);
}

Component* GameObject::GetComponent(CO_TYPE type)
{
	return components[type];
}

/*RETURNS ONLY THE FIRST COMPONENT OF THE FIRST CHILDREN THAT HAS IT*/
Component* GameObject::GetComponentInChildren(CO_TYPE type)
{
	Component* toReturn = nullptr;

	for (int i = 0; i < children.size(); ++i)
	{
		toReturn = children[i]->GetComponent(type);

		if (toReturn != nullptr) return toReturn;
	}
	return nullptr; 
}
