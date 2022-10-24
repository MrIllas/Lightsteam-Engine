#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Color.h"
//#include "glmath.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/float4x4.h"

class Shader;

class Light
{
public:
	Light();
	~Light();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	float3 position;

	int ref = 0;
	bool on = false;

public:
	float3 GetLightPosition() { return lightPosition; }
	float3 GetLightColour() { return lightColour; }
	float GetSpecularStrength() { return specularStrength; }

	void SetAmbientLight(float3 pos) { lightPosition = pos; }
	void SetDiffuseLight(float3 colour) { lightColour = colour; }
	void SetSpecularStrength(float strength) { specularStrength = strength; }

	void SetShaderData(Shader* shader);

private:
	float3 lightPosition;
	float3 lightColour;

	float specularStrength;
};

#endif // !__LIGHT_H__