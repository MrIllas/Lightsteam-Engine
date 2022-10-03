#include "Primitive.h"

Primitive::Primitive()
{
}

Primitive::~Primitive()
{

}

void Primitive::Init(PrimitiveTypes primitiveType, float3 position, float3 size)
{
	this->position = position;
	this->size = size;

	switch (primitiveType)
	{
	case PrimitiveTypes::CUBE:
		Cube();
		break;
	}
}

void Primitive::Cube()
{

	float x, y, z;
	x = size.x * 1.0f;
	y = size.y * 1.0f;
	z = size.z * 1.0f;

	vertex = std::vector<float3>
	{
		{x, y, z},
		{-x, y, z},
		{-x, -y, z},
		{x, -y, z},
		{x, -y, -z},
		{x, y, -z},
		{-x, y, -z},
		{-x, -y, -z}
	};

	index = std::vector<uint>
	{
		0,1,2,2,3,0,
		0,3,4,4,5,0,
		0,5,6,6,1,0,
		6,5,7,7,4,5,
		1,6,7,7,2,1,
		7,2,3,3,4,7
	};
}