#pragma once

#include "Globals.h"

#include <vector>
#include <iostream>



struct Log
{
	std::string log;
	LOG_TYPE type;
	std::string time;
};

class Loggs
{
public:
	Loggs(){}

	static Loggs* Instance();
	static bool HasInstance();

	static void Delete();

	const char* GetString(uint i) { return log[i].log.c_str(); }
	LOG_TYPE GetType(uint i) { return log[i].type; }
	const char* GetTime(uint i) { return log[i].time.c_str(); }
	void AddLog(std::string ss, LOG_TYPE tt, std::string time) {
		Log aux;
		aux.log = ss;
		aux.type = tt;
		aux.time = time;
		log.emplace_back(aux);
	}
	uint Size() { return log.size(); }

private:
	std::vector<Log> log;

	static Loggs* instance;
};