#include "GameObject.h"

#include "CompTransform.h"
#include "CompMeshRenderer.h"

GameObject::GameObject(std::string name, bool spatial)
{
	this->name = name;
	selected = false;

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
	if (components.empty()) return;

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
		LOG(LOG_TYPE::ATTENTION, "A component of type %s already exists in %s !", std::to_string(type).c_str(), name.c_str());
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
	if (components.empty()) return;

	Component* comp = components[type];

	if (comp != nullptr)
	{
		RELEASE(comp);
	}
	components.erase(type);
}

Component* GameObject::GetComponent(CO_TYPE type)
{
	if (components.empty()) return nullptr;
	return components[type];
}

/*RETURNS ONLY THE FIRST COMPONENT OF THE FIRST CHILDREN THAT HAS IT*/
Component* GameObject::GetComponentInChildren(CO_TYPE type)
{
	if (children.empty()) return nullptr;
	Component* toReturn = nullptr;

	for (int i = 0; i < children.size(); ++i)
	{
		toReturn = children[i]->GetComponent(type);

		if (toReturn != nullptr) return toReturn;
	}
	return nullptr; 
}

void GameObject::AddChildren(GameObject* go)
{
	children.emplace_back(go);
	if (go->parent == nullptr) go->parent = this;
}

void GameObject::RemoveChildren(GameObject* go)
{
	if (children.empty()) return;
	for (int i = 0; i < children.size(); ++i)
	{
		GameObject* aux = children[i];
		if (*children[i] == *go)
		{
			children.erase(children.begin() + i);
		}
	}
}

void GameObject::SetParent(GameObject* go)
{
	if (parent != nullptr)
	{
		if (go->parent == this) return;
		parent->RemoveChildren(this);
	}
	parent = go;
	parent->children.emplace_back(this);
}