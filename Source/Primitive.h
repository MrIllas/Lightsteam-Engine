#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Globals.h"

#include "MathGeoLib/include/Math/float3.h"
#include "Color.h"
#include <vector>

enum PrimitiveTypes
{
	CUBE
};

class Primitive
{
public:
	Primitive();
	~Primitive();

	void Init(PrimitiveTypes primitiveType, float3 position = { 0, 0, 0 }, float3 size = { 1, 1, 1});

	std::vector<float3> GetVertex() { return vertex; };
	std::vector<uint> GetIndex() { return index; };

private:
	void Cube();
	
	float3 size = { 1, 1, 1};
	float3 position = { 0, 0, 0}; 

private:
	std::vector<float3> vertex;
	std::vector<uint> index;

};
#endif // !__PRIMITIVE_H__