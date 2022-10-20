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

struct LogCollapse
{
	Log log;
	int count;
};

class Loggs
{
public:
	Loggs(){}

	static Loggs* Instance();
	static bool HasInstance();

	static void Delete();

	const char* GetString(uint i, bool collapse = false) { return collapse? logCollapse[i].log.log.c_str() : log[i].log.c_str(); }
	LOG_TYPE GetType(uint i, bool collapse = false) { return collapse? logCollapse[i].log.type : log[i].type; }
	const char* GetTime(uint i, bool collapse = false) { return collapse? logCollapse[i].log.time.c_str() : log[i].time.c_str(); }
	void AddLog(std::string ss, LOG_TYPE tt, std::string time) {
		Log aux;
		aux.log = ss;
		aux.type = tt;
		aux.time = time;
		log.emplace_back(aux);
		AddLogCollapse(aux);
	}
	uint Size(bool collapse = false) { return collapse? logCollapse.size() :log.size(); }
	int GetCollapseCount(uint i) { return logCollapse[i].count; }

private:
	void AddLogCollapse(Log log);

private:
	std::vector<Log> log;
	std::vector<LogCollapse> logCollapse;

	static Loggs* instance;
};