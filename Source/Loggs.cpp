#include "Loggs.h"
#include "Globals.h"

#include <atlstr.h>

Loggs* Loggs::instance = nullptr;

Loggs* Loggs::Instance()
{
	if (instance == nullptr) instance = new Loggs();
	return instance;
}

bool Loggs::HasInstance()
{
	if (instance == nullptr)return false;
	else return true;
}

void Loggs::Delete()
{
	if (instance != nullptr) RELEASE(instance);
}

void log(const char file[], int line, LOG_TYPE type, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);

	//Time
	std::string stt;
	CString cstrMessage;
	SYSTEMTIME st;

	GetLocalTime(&st);

	cstrMessage.Format("[%02d:%02d:%02d]",
		st.wHour,
		st.wMinute,
		st.wSecond);

	stt = cstrMessage;

	if(Loggs::HasInstance()) Loggs::Instance()->AddLog(tmp_string, type, stt);
}

void Loggs::AddLogCollapse(Log log)
{
	for (int i = 0; i < logCollapse.size(); ++i)
	{
		if (logCollapse[i].log.log.compare(log.log) == 0)
		{
			logCollapse[i].count++;
			logCollapse[i].log.time = log.time;
			return;
		}
	}

	LogCollapse logC;
	logC.count = 1;
	logC.log = log;
	logCollapse.emplace_back(logC);
}
