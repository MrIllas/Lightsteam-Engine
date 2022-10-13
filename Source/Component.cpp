#include "Component.h"

#include "GameObject.h"


Component::Component(GameObject* owner)
{
	this->owner = owner;
	active = true;
	type = CO_TYPE::NONE;
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

void Component::Enable()
{
	active = true;
}

void Component::Disable()
{
	active = true;
}

