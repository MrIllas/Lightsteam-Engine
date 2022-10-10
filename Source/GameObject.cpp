#include "GameObject.h"

GameObject::GameObject()
{
	name = "GameObject";
}

GameObject::~GameObject()
{

}

void GameObject::Update()
{

}

Component* GameObject::CreateComponent(CO_TYPE type)
{
	if (CheckComponents(type))
	{
		LOG("A component of type %s already exists in %s !", std::to_string(type).c_str(), name.c_str());
		return nullptr; 
	}

	Component* toReturn = nullptr;

	switch (type)
	{
		case TRANSFORM:
			//toReturn = new Component();
			break;
		case MESH:

			break;
		case MATERIAL:

			break;
	}

	if(toReturn != nullptr) components.emplace_back(toReturn);

	return toReturn;
}

Component* GameObject::GetComponent()
{

	return nullptr;
}

Component* GameObject::GetComponentInChildren()
{

}

bool GameObject::CheckComponents(CO_TYPE type)
{
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
		{
			return true;
		}
	}
	return false;
}