#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define NOMINMAX //Solves an error with Rapid Json where the lib tries to use a min max macro but it gets the windows.h macro
#include <windows.h>
#include <stdio.h>
#include <time.h> 

enum class LOG_TYPE
{
	NONE,
	ERRO,
	SUCCESS,
	ATTENTION,
	ENGINE
};

#define LOG(type, format, ...) log(__FILE__, __LINE__, type, format, __VA_ARGS__);

void log(const char file[], int line, LOG_TYPE type, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != nullptr )        \
        {                      \
      delete x;            \
      x = nullptr;              \
        }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{							  \
	if( x != nullptr )			  \
		{							\
	  delete[] x;				\
	  x = nullptr;					\
		}							\
	}

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
};

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

enum UpdateStatus
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

#endif // !__GLOBALS_H__