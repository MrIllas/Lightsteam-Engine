#include "CompTransform.h"


CompTransform::CompTransform(GameObject* owner) : Component (owner)
{
	this->type = CO_TYPE::TRANSFORM;
}

CompTransform::~CompTransform()
{

}

void CompTransform::Init()
{

}

void CompTransform::Update()
{

}