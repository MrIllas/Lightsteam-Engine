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
	//Engine version
	std::string vEngine;

	//Lib versions
	std::string vSdl;
	std::string vImGui;
	std::string vImGuiFileDialog;
	std::string vImGuiColorTextEdit;
	std::string vMathGeoLib;
	std::string vOpenGl;
	std::string vGlew;
	std::string vNlohmann;
	std::string vAssimp;
	std::string vPhysfs;
	std::string vPugiXml;
};

