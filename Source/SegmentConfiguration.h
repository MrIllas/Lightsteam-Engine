#pragma once
#include "Segment.h"

#include <iostream>
#include <queue>

class Time;
class RenderProperties;
class WindowProperties;
class EditorProperties;

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
	void EditorHeader();

	void GetCaps();

private:
	ImGuiIO io;
	RenderProperties* rProps = nullptr;
	WindowProperties* wProps = nullptr;
	EditorProperties* eProps = nullptr;
	Time* time = nullptr;

	bool vsync = false;

	/*std::string strCaps;*/
	std::vector<float> framerateQueue;
	std::vector<float> milisecondsQueue;
};

