#pragma once

#include <string> 

enum CO_TYPE
{
	EMPTY,
	TRANSFORM,
	MESH,
	MATERIAL
};


class Component
{
public:
	Component(){};
	~Component(){};

	virtual void Enable() { enabled = true; };
	virtual void Disable() { enabled = false; };

	virtual void Update() {};

public:
	CO_TYPE type = CO_TYPE::EMPTY;
	bool enabled = true;
	
	GameObject* owner = nullptr;
};

