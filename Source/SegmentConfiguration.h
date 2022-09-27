#pragma once
#include "Segment.h"
#include "Application.h"
#include "ModuleWindow.h"


#include <iostream>
#include <queue>

class SegmentConfiguration : public Segment
{
public:
	SegmentConfiguration(std::string name, bool enabled = false);
	~SegmentConfiguration();

	void Update() override;

private:
	void ApplicationHeader();
	void WindowHeader();
	void InputHeader();
	void RenderingHeader();

	void GetCaps();

private:
	ImGuiIO io;
	WindowProperties* wProps = nullptr;
	Time* time = nullptr;

	bool vsync = false;

	/*std::string strCaps;*/
	std::vector<float> framerateQueue;
	std::vector<float> milisecondsQueue;
};

