#include "Mesh.h"

#include "glmath.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::Init(PrimitiveTypes primitiveType, float3 position, float3 size)
{
	this->position = position;
	this->size = size;

	switch (primitiveType)
	{
		case CUBE:
			Cube();
			break;
		case SPHERE:
			Sphere();
			break;
		case PYRAMID:
			Pyramid();
			break;
		case CYLINDER:
			Cylinder();
			break;
	}
}

void Mesh::InitMesh()
{

}

#pragma region Primitives
void Mesh::Cube()
{
	//float x, y, z;
	//x = size.x * 1.0f;
	//y = size.y * 1.0f;
	//z = size.z * 1.0f;

	//vertex = std::vector<float3>
	//{
	//	{x, y, z},
	//	{-x, y, z},
	//	{-x, -y, z},
	//	{x, -y, z},
	//	{x, -y, -z},
	//	{x, y, -z},
	//	{-x, y, -z},
	//	{-x, -y, -z}
	//};

	////Fix index Cullaface
	//index = std::vector<uint>
	//{
	//	0,1,2,2,3,0,
	//	0,3,4,4,5,0,
	//	0,5,6,6,1,0,
	//	6,5,7,7,4,5,
	//	1,6,7,7,2,1,
	//	7,2,3,3,4,7
	//};
}

void Mesh::Sphere()
{
	//float radius = 1.0f;
	//int rings = 15;
	//int sectors = 15;

	//float const R = 1. / (float)(rings - 1);
	//float const S = 1. / (float)(sectors - 1);

	//for (int r = 0; r < rings; ++r)
	//{
	//	for (int s = 0; s < sectors; ++s)
	//	{
	//		float const y = sin(-M_PI_2 + M_PI * r * R);
	//		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
	//		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

	//		//texcoords.push_back(float2(s * S, r * R));
	//		//Vertex
	//		vertex.push_back(float3(x, y, z) * radius);

	//		if (r < rings - 1)
	//		{
	//			int curRow = r * sectors;
	//			int nextRow = (r + 1) * sectors;
	//			int nextS = (s + 1) % sectors;

	//			//Index
	//			index.push_back(curRow + s);
	//			index.push_back(nextRow + s);
	//			index.push_back(nextRow + nextS);

	//			index.push_back(curRow + s);
	//			index.push_back(nextRow + nextS);
	//			index.push_back(curRow + nextS);
	//		}
	//	}
	//}
}

void Mesh::Pyramid()
{
	/*float x, y, z;
	x = size.x * 1.0f;
	y = size.y * 1.0f;
	z = size.z * 1.0f;

	vertex = std::vector<float3>
	{
		{0, y, 0},
		{-x, -y, z},
		{x, -y, z},
		{x, -y, -z},
		{-x , -y, -z}
	};

	index = std::vector<uint>
	{
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,1,
		2,1,4,2,4,3
	};*/
}

void Mesh::Cylinder()
{
	//int sector = 3;
	//float height = 1.0f;
	//float radius = 0.5f;

	//float sectorStep = 2 * M_PI / sector;
	//float sectorAngle;  // radian

	//std::vector<float> unitVertices;
	//for (int i = 0; i <= sector; ++i)
	//{
	//	sectorAngle = i * sectorStep;
	//	unitVertices.push_back(cos(sectorAngle)); // x
	//	unitVertices.push_back(sin(sectorAngle)); // y
	//	unitVertices.push_back(0);                // z
	//}

	//// put side vertices to arrays
	//for (int i = 0; i < 2; ++i)
	//{
	//	float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
	//	float t = 1.0f - i;                              // vertical tex coord; 1 to 0

	//	for (int j = 0, k = 0; j <= sector; ++j, k += 3)
	//	{
	//		float ux = unitVertices[k];
	//		float uy = unitVertices[k + 1];
	//		float uz = unitVertices[k + 2];
	//		// position vector
	//		vertex.push_back(float3(ux * radius , uy * radius, h));             
	//		// normal vector
	//		//normals.push_back(ux);                       // nx
	//		//normals.push_back(uy);                       // ny
	//		//normals.push_back(uz);                       // nz
	//		//// texture coordinate
	//		//texCoords.push_back((float)j / sectorCount); // s
	//		//texCoords.push_back(t);                      // t
	//	}
	//}

	//// the starting index for the base/top surface
	////NOTE: it is used for generating indices later
	//int baseCenterIndex = (int)vertex.size() / 3;
	//int topCenterIndex = baseCenterIndex + sector + 1; // include center vertex

	//// put base and top vertices to arrays
	//for (int i = 0; i < 2; ++i)
	//{
	//	float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
	//	float nz = -1 + i * 2;                           // z value of normal; -1 to 1

	//	// center point
	//	vertex.push_back(float3(0, 0, h));
	//	/*normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
	//	texCoords.push_back(0.5f); texCoords.push_back(0.5f);*/

	//	for (int j = 0, k = 0; j < sector; ++j, k += 3)
	//	{
	//		float ux = unitVertices[k];
	//		float uy = unitVertices[k + 1];
	//		// position vector
	//		vertex.push_back(float3(ux * radius, uy * radius, h));
	//		// normal vector
	//		//normals.push_back(0);                        // nx
	//		//normals.push_back(0);                        // ny
	//		//normals.push_back(nz);                       // nz
	//		//// texture coordinate
	//		//texCoords.push_back(-ux * 0.5f + 0.5f);      // s
	//		//texCoords.push_back(-uy * 0.5f + 0.5f);      // t
	//	}
	//}

	////index

	//// generate CCW index list of cylinder triangles
	//int k1 = 0;                         // 1st vertex index at base
	//int k2 = sector + 1;           // 1st vertex index at top

	//// indices for the side surface
	//for (int i = 0; i < sector; ++i, ++k1, ++k2)
	//{
	//	// 2 triangles per sector
	//	// k1 => k1+1 => k2
	//	index.push_back(k1);
	//	index.push_back(k1 + 1);
	//	index.push_back(k2);

	//	// k2 => k1+1 => k2+1
	//	index.push_back(k2);
	//	index.push_back(k1 + 1);
	//	index.push_back(k2 + 1);
	//}

	//// indices for the base surface
	//for (int i = 0, k = baseCenterIndex + 1; i < sector; ++i, ++k)
	//{
	//	if (i < sector - 1)
	//	{
	//		index.push_back(baseCenterIndex);
	//		index.push_back(k + 1);
	//		index.push_back(k);
	//	}
	//	else // last triangle
	//	{
	//		index.push_back(baseCenterIndex);
	//		index.push_back(baseCenterIndex + 1);
	//		index.push_back(k);
	//	}
	//}


	//// indices for the top surface
	//for (int i = 0, k = topCenterIndex + 1; i < sector; ++i, ++k)
	//{
	//	if (i < sector - 1)
	//	{
	//		index.push_back(topCenterIndex);
	//		index.push_back(k);
	//		index.push_back(k + 1);
	//	}
	//	else // last triangle
	//	{
	//		index.push_back(topCenterIndex);
	//		index.push_back(k);
	//		index.push_back(topCenterIndex + 1);
	//	}
	//}
}
#pragma endregion Primitives