#include "Globals.h"
#include "Light.h"
#include <gl/GL.h>
//#include <gl/GLU.h>

#include "Shader.h"

Light::Light() : ref(-1), on(false), position(0.0f, 0.0f, 0.0f)
{
	ref = -1;
	on = false;
	position = float3(0.0f, 0.0f, 0.0f);

	lightPosition = float3(0.0f, 5.0f, -5.0f);
	lightColour = float3(1.0f, 1.0f, 1.0f);
	specularStrength = 0.0f;
}

Light::~Light()
{

}

void Light::Init()
{
	/*glLightfv(ref, GL_AMBIENT, &ambient);
	glLightfv(ref, GL_DIFFUSE, &diffuse);*/
}

void Light::SetPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Light::Render()
{
	if(on)
	{
		float pos[] = {position.x, position.y, position.z, 1.0f};
		glLightfv(ref, GL_POSITION, pos);
	}
}

void Light::Active(bool active)
{
	if(on != active)
	{
		on = !on;

		if(on)
			glEnable(ref);
		else
			glDisable(ref);
	}
}

void Light::SetShaderData(Shader* shader)
{
	shader->SetVec3("lightPos", &lightPosition[0]);
	shader->SetVec3("lightColour", &lightColour[0]);
	shader->SetFloat("specularStrength", specularStrength);
}