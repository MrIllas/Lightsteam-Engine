#include "SegmentShaderText.h"

#include "ModuleResources.h"
#include "ShaderManager.h"

SegmentShaderText::SegmentShaderText(bool enabled) : Segment(enabled)
{
	name = "Shader Text Editor";
}

SegmentShaderText::~SegmentShaderText()
{

}

void SegmentShaderText::Start()
{
	resInstance = ResourceProperties::Instance();
}

void SegmentShaderText::Update()
{
	if (ImGui::Begin(name.c_str()))
	{
		std::string aux = currentResource->GetName();
		ImGui::Text(aux.c_str());
	}
	ImGui::End();
}

void SegmentShaderText::SetResource(std::string resourceUuid)
{
	this->currentResource = (ResourceShader*) ResourceProperties::Instance()->resources.at(resourceUuid);
}