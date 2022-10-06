#pragma once
#include "Segment.h"

class SegmentAbout : public Segment
{
public:
	SegmentAbout(bool enabled = false);
	~SegmentAbout();

	void Update() override;

public:
	//About Text
	void ThirdPartyLibs();
	void MyLicense();

private:
	
	//Lib versions
	std::string vSdl;
	std::string vImGui;
	std::string vMathGeoLib;
	std::string vOpenGl;
	std::string vGlew;
	std::string vSimdjson;
	std::string vAssimp;
	std::string vPhysfs;
	std::string vPugiXml;
};

