#pragma once
#include "Component.h"

#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"

class CompTransform : public Component
{
public:
	CompTransform(GameObject* owner);
	~CompTransform();

	void Init() override;
	void Update() override;

public:

	float4x4 localTransform;
	float4x4 globalTransform;

	float3 position;
	float3 rotation;
	float3 localScale;
};

