#include "GameObject.h"

#include "CompTransform.h"
#include "CompMeshRenderer.h"
#include "CompTexture.h"
#include "CompCamera.h"
#include "CompMaterial.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "LibraryManager.h"
#include "LSUUID.h"

GameObject::GameObject(std::string name, bool spatial)
{
	this->name = name;
	this->uuid = LS_UUID::Generate();
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

void GameObject::Init()
{
	if (components.empty()) return;

	for (auto const& comp : components)
	{
		if (comp.second->active)
		{
			comp.second->Init();
		}
	}
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

void GameObject::UpdateCompMenuGUI()
{
	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	
	if (ImGui::BeginCombo(" ", "Add Component", ImGuiComboFlags_PopupAlignLeft))
	{
		for (int compType = TRANSFORM; compType != LAST; compType++)
		{
			CO_TYPE coType = static_cast<CO_TYPE>(compType);
			
			if (GetComponent(coType) == nullptr) //Checks if the components is already added
			{
				if (ImGui::Selectable(CompTypeToString(coType)))
				{
					CreateComponent(coType);
				}
			}
			
		}

		ImGui::EndCombo();
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
			toReturn = new CompTransform(this, LS_UUID::Generate());
			break;
		case MESH_RENDERER:
			toReturn = new CompMeshRenderer(this, LS_UUID::Generate());
			break;
		case MATERIAL:
			toReturn = new CompTexture(this, LS_UUID::Generate());
			break;
		case CAMERA:
			toReturn = new CompCamera(this, LS_UUID::Generate());
			break;
		case TESTING:
			toReturn = new CompMaterial(this, LS_UUID::Generate());
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
	if (components.count(type)) return components[type];
	else return nullptr;
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

std::vector<GameObject*> GameObject::GetChildrens()
{
	return children;
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
		//if (go->parent == this) return;
		if (CheckParentsOfParent(go, this)) return;

		parent->RemoveChildren(this);
	}
	parent = go;
	parent->children.emplace_back(this);
}

bool GameObject::CheckParentsOfParent(GameObject* go, GameObject* checkGO)
{
	if (go->parent == nullptr) return false;
	else
	{
		if (go->parent == checkGO) return true;
		else return CheckParentsOfParent(go->parent, checkGO);
	}
}

void GameObject::DeleteGameObject()
{
	parent->RemoveChildren(this);
	for (int i = 0; i < children.size(); ++i)
	{
		if (children[i] != nullptr)
		{
			children[i]->DeleteGameObject();
			RELEASE(children[i]);
		}
	}

	this->~GameObject();
	children.clear();
}

std::string GameObject::GetUUName()
{
	std::string toReturn = name;
	toReturn += "##";
	toReturn += uuid;

	return toReturn.c_str();
}

#pragma region Save/Load
nlohmann::ordered_json GameObject::Save()
{
	nlohmann::JsonData data;

	data.SetString("Name", this->name);
	data.SetString("UUID", this->uuid);
	if(this->parent != nullptr) data.SetString("Parent_UUID", this->parent->uuid);

	std::vector<nlohmann::ordered_json> aux;
	for (auto const& comp : components)
	{
		aux.push_back(comp.second->Save());
	}
	data.data.emplace("Components", aux);

	/*std::string savePath = LIB_MODELS;
	savePath += "/";
	savePath += this->uuid;

	LibraryManager::SaveJSON(savePath, data.data.dump(4)); */

	return data.data;
}

void GameObject::Load(nlohmann::JsonData data)
{
	this->name = data.GetString("Name");
	this->uuid = data.GetString("UUID");
	//if (this->parent != nullptr) this->parent->uuid = data.GetString("Parent_UUID");

	if (data.data.contains("Components"))
	{
		std::vector<nlohmann::ordered_json> aux;

		aux = data.data["Components"].get<nlohmann::ordered_json>();

		if (aux.size() > 0)
		{
			for (int i = 0; i < aux.size(); ++i)
			{
				nlohmann::JsonData compData;
				compData.data = aux.at(i);

				if (compData.data.contains("Type"))
				{
					Component* theComp = CreateComponent((CO_TYPE)compData.GetInt("Type"));
					theComp->Load(compData);
				}
			}
		}
	}
}

#pragma endregion Save & Load