#ifndef __POINT_H__
#define __POINT_H__

#include "Globals.h"
#include <math.h>

template<class TYPE>
class Point
{
public:

	TYPE x = 0, y = 0;

	Point() : x(0), y(0)
	{}

	Point(const Point& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	Point(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	Point& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	Point operator - (const Point& v) const
	{
		Point r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	Point operator + (const Point& v) const
	{
		Point r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	Point operator * (const TYPE num) const
	{
		Point r;

		r.x = x * num;
		r.y = y * num;

		return(r);
	}

	const Point& operator -=(const Point& v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const Point& operator +=(const Point& v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	const Point& operator +=(int num)
	{
		x += num;
		y += num;

		return(*this);
	}

	Point operator *=(const int num)
	{
		x *= num;
		y *= num;

		return(*this);
	}

	bool operator ==(const Point& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const Point& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	Point& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	Point& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	Point Normalize()
	{
		Point result;

		TYPE magnitud = sqrt(pow(x, 2) + pow(y, 2));

		if (magnitud == 0) return { 0,0 };

		result = { x / magnitud, y / magnitud };

		return result;
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (TYPE)sqrtf(float(fx * fx) + float(fy * fy));
	}

	TYPE DistanceNoSqrt(const Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx * fx) + (fy * fy);
	}

	TYPE DistanceManhattan(const Point& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}
};

typedef Point<int> iPoint;
typedef Point<float> fPoint;

#endif // __POINT_H__