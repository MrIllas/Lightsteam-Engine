#pragma once
#include "Segment.h"

class ResourceShader;
struct ResourceProperties;

class SegmentShaderText : public Segment
{
public:
	SegmentShaderText(bool enabled = false);
	~SegmentShaderText() override;

	void Start() override;
	void Update() override;

	void SetResource(std::string resourceUuid);

private:
	ResourceShader* currentResource = nullptr;

	ResourceProperties* resInstance = nullptr;

};

