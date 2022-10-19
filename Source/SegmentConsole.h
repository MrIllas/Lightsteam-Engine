#pragma once
#include "Segment.h"

//#include "Globals.h"


class Loggs;

class SegmentConsole : public Segment
{
public:
	SegmentConsole(bool enabled = false);
	~SegmentConsole();

	void Update() override;

private:
	Loggs* loggs;

	int lastSize = 0;
	bool scrollDown = false;
	bool showTime = true;
	bool collapse = false;
	

	bool logError = true;
	bool logAttention = true;
	bool logEngine = true;
	bool logSuccess = true;
};

