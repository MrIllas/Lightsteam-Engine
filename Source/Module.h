#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"

#include "External/PugiXml/src/pugixml.hpp"

class PhysBody3D;
class Application;

class Module
{
private:
	bool enabled = false;

public:
	std::string name = "NONAME";

	Application* App = nullptr;

	pugi::xml_node config;

public:
	Module();

	Module(Application* parent, bool start_enabled = true);

	virtual ~Module();

	virtual bool Init();

	virtual bool Start();

	virtual UpdateStatus PreUpdate();

	virtual UpdateStatus Update();

	virtual UpdateStatus PostUpdate();

	virtual bool CleanUp();

	virtual void SaveSettingsData(pugi::xml_node& save);

	virtual void LoadSettingsData(pugi::xml_node& load);
};

#endif // !__MODULE_H__