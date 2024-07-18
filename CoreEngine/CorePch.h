#pragma once
#pragma comment(lib,"ws2_32.lib")

#include "GameData.h"
#include "Type.h"
#include "Packet.h"
#include "Macro.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#define _WINSOCKAPI_
#include <Windows.h>

#include <vector>
#include <unordered_map>
#include <list>

#include <atlimage.h>
#include <time.h>


struct FRECT
{
	float left;
	float top;
	float right;
	float bottom;

	// operator overloading
	FRECT operator+(FRECT other) const
	{
		FRECT temp;

		temp.left += other.left;
		temp.top += other.top;
		temp.right += other.right;
		temp.bottom += other.bottom;

		return temp;
	}
	FRECT operator-(FRECT other) const
	{
		FRECT temp;
		temp.left -= other.left;
		temp.top -= other.top;
		temp.right -= other.right;
		temp.bottom -= other.bottom;

		return temp;
	}

	void operator=(const FRECT& other)
	{
		left = other.left;
		top = other.top;
		right = other.right;
		bottom = other.bottom;
	}

	void operator=(const RECT& other)
	{
		left = (float)other.left;
		top = (float)other.top;
		right = (float)other.right;
		bottom = (float)other.bottom;
	}

	operator RECT () const
	{
		RECT temp;
		temp.left = (int)left;
		temp.top = (int)top;
		temp.right = (int)right;
		temp.bottom = (int)bottom;

		return temp;
	}

	// function
	bool IntersectRect(FRECT other) const
	{
		if (left < other.right &&
			top < other.bottom &&
			right > other.left &&
			bottom > other.top)
		{
			return true;
		}

		return false;
	}
	bool IntersectRect(RECT other) const
	{
		FRECT temp;
		temp.operator=(other);

		return temp.IntersectRect(*this);
	}

	void OffsetRect(float dx, float dy)
	{
		left += dx;
		right += dx;
		top += dy;
		bottom += dy;
	}
	void OffsetRect(int dx, int dy)
	{
		left += dx;
		right += dx;
		top += dy;
		bottom += dy;
	}
};