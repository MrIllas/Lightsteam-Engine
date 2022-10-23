#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Color.h"
//#include "glmath.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/float4x4.h"

struct Light
{
	Light();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	Color ambient;
	Color diffuse;
	float3 position;

	int ref = 0;
	bool on = false;
};

#endif // !__LIGHT_H__