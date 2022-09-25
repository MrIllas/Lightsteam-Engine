#pragma once
#include "Segment.h"
#include "ModuleWindow.h"

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

private:
	ImGuiIO io;
	WindowProperties* wProps = nullptr;
};

