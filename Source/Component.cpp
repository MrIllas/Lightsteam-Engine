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

#pragma region Save/Load


nlohmann::ordered_json Component::Save()
{
	nlohmann::JsonData data;

	data.SetString("Type", CompTypeToString(type));
	data.SetString("UUID", uuid);
	data.SetBool("Active", active);

	return SaveUnique(data);
}

nlohmann::ordered_json Component::SaveUnique(nlohmann::JsonData data)
{
	return data.data;
}

void Component::Load(nlohmann::json data)
{

}

#pragma endregion Save & Load