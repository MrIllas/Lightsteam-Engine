#pragma once

#include "Globals.h"

#include <vector>
#include <iostream>

class Loggs
{
public:
	Loggs(){}

	static Loggs* Instance();
	static bool HasInstance();

	static void Delete();

	const char* Get(uint i) { return log[i].c_str(); }
	void AddLog(std::string ss) { log.emplace_back(ss); }
	uint Size() { return log.size(); }

private:
	std::vector<std::string> log;

	static Loggs* instance;
};