#pragma once
#include "Component.h"

#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

class CompTransform : public Component 
{
public:
	CompTransform(GameObject* owner);
	~CompTransform() override;

	void Init() override;
	void Update() override;

	void UpdateGUI() override;

	float4x4 GetWorldMatrix();

	Quat GetRotationAsQuat();

public:

	float4x4 localTransform;
	float4x4 globalTransform;

	float3 position;
	//Quat rotation;
	float3 rotation;
	float3 localScale;
};

