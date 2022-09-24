#pragma once
#include "Segment.h"

class SegmentAbout : public Segment
{
public:
	SegmentAbout(std::string name, bool enabled = false);
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
	std::string vRapidJson;
};

