#include "Loggs.h"
#include "Globals.h"

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

void log(const char file[], int line, const char* format, ...)
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

	if(Loggs::HasInstance()) Loggs::Instance()->AddLog(tmp_string);
}