#pragma once
#include "Segment.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include <iostream>
#include <queue>

class SegmentConfiguration : public Segment
{
public:
	SegmentConfiguration(bool enabled = false);
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
	RenderProperties* rProps = nullptr;
	WindowProperties* wProps = nullptr;
	Time* time = nullptr;

	bool vsync = false;

	/*std::string strCaps;*/
	std::vector<float> framerateQueue;
	std::vector<float> milisecondsQueue;
};

