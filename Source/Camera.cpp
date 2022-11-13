#include "Camera.h"
#include "Globals.h"

#include "Renderer.h"

#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/Quat.h"

Camera::Camera()
{
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f, -10.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.verticalFov = DegToRad(60.0f);
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f);
	
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 500.0f;
	frustum.front = Z;
	frustum.up = Y;

	frustum.pos = Position;

	//if(renderer == nullptr) renderer = new Renderer({ 720, 360 });
}

Camera::~Camera()
{
	if (renderer != nullptr) RELEASE(renderer);
}

void Camera::Look(const float3& Position, const float3& Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Cross(Z, X); //cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}
}

void Camera::LookAt(const float3& Spot)
{
	frustum.front = (Spot - frustum.pos).Normalized();
	frustum.up = frustum.front.Cross(float3(.0f, 1.0f, .0f).Cross(frustum.front).Normalized()); // normalize(cross(float3(0.0f, 1.0f, 0.0f), Z));
}

void Camera::Move(const float3& Movement)
{
	frustum.pos = Movement;
}

float* Camera::GetViewMatrix()
{
	float4x4 aux = frustum.ViewMatrix();
	aux.Transpose(); 
	return &aux.v[0][0];
}

float* Camera::GetProjectionMatrix()
{
	float4x4 aux = frustum.ProjectionMatrix();
	aux = aux.Transposed();
	return &aux.v[0][0];
}

void Camera::SetRenderer(float2 size)
{
	renderer = new Renderer(size, this);
}

//Quat Camera::GetRotationQuat()
//{
//	Quat toReturn = Quat::identity;
//	toReturn.toQuat
//}