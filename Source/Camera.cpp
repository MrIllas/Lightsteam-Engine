#include "Camera.h"
#include "Globals.h"

#include "Renderer.h"

#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Geometry/Plane.h"

Camera::Camera()
{
	X = float3(-1.0f, 0.0f, 0.0f);
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
}

Camera::~Camera()
{
	RELEASE(renderer);
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
	ViewMatrix = frustum.ViewMatrix();
	ViewMatrix.Transpose();
	//float4x4 aux = frustum.ViewMatrix();
	///aux.Transpose(); 
	return &ViewMatrix.v[0][0];
}

float* Camera::GetProjectionMatrix()
{
	ProjectionMatrix = frustum.ProjectionMatrix().Transposed();
	/*float4x4 aux = frustum.ProjectionMatrix();
	aux = aux.Transposed();*/
	return &ProjectionMatrix.v[0][0];
}

void Camera::SetRenderer(float2 size)
{
	renderer = new Renderer(size, this);
}

bool Camera::ContainsBBox(AABB& bBox)
{
	float3 vCorner[8];
	int iTotalIn = 0;

	math::Plane frustumPlanes[6];
	frustum.GetPlanes(frustumPlanes);

	bBox.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array
	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (frustumPlanes[p].IsOnPositiveSide(vCorner[i])) { //<-- “IsOnPositiveSide” from MathGeoLib
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if(iInCount == 0)
			return false;
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	return true;
}

//Quat Camera::GetRotationQuat()
//{
//	Quat toReturn = Quat::identity;
//	toReturn.toQuat
//}