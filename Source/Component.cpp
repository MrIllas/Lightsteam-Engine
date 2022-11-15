#include "Component.h"

#include "GameObject.h"


Component::Component(GameObject* owner, std::string uuid)
{
	this->owner = owner;
	this->uuid = uuid;
	active = true;
	type = CO_TYPE::NONE;
	deleteQueue = false;
}

Component::~Component()
{

}

void Component::Init()
{

}

void Component::Update()
{

}

void Component::UpdateGUI()
{

}

void Component::Enable()
{
	active = true;
}

void Component::Disable()
{
	active = true;
}

