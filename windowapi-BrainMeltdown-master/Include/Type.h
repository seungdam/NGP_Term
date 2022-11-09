#pragma once

#include <Windows.h>

// 실수형 사각형
struct FRECT {
	float left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;

	// operator overloading
	FRECT operator+(FRECT other) const {
		FRECT temp;
		temp.left += other.left;
		temp.top += other.top;
		temp.right += other.right;
		temp.bottom += other.bottom;

		return temp;
	}
	FRECT operator-(FRECT other) const {
		FRECT temp;
		temp.left -= other.left;
		temp.top -= other.top;
		temp.right -= other.right;
		temp.bottom -= other.bottom;

		return temp;
	}

	void operator=(FRECT other) {
		left = other.left;
		top = other.top;
		right = other.right;
		bottom = other.bottom;
	}
	void operator=(RECT other) {
		left = (float)other.left;
		top = (float)other.top;
		right = (float)other.right;
		bottom = (float)other.bottom;
	}

	operator RECT () const {
		RECT temp;
		temp.left = (int)left;
		temp.top = (int)top;
		temp.right = (int)right;
		temp.bottom = (int)bottom;

		return temp;		
	}

	// function
	BOOL IntersectRect(FRECT other) const {
		if (left < other.right &&
			top < other.bottom &&
			right > other.left &&
			bottom > other.top)		return TRUE;

		return FALSE;
	}
	BOOL IntersectRect(RECT other) const {
		FRECT temp;

		temp.operator=(other);
		return temp.IntersectRect(*this);
	}

	void OffsetRect(float dx, float dy) {
		left += dx;
		right += dx;
		top += dy;
		bottom += dy;
	}
	void OffsetRect(int dx, int dy) {
		left += dx;
		right += dx;
		top += dy;
		bottom += dy;
	}

};

struct FPOINT {
	float x, y;
};

// enum
// Move Direction
enum class MOVE_DIR {
	MD_BACK = -1,
	MD_NONE,
	MD_FRONT,
	MD_UP = -10,
	MD_DOWN = 10
};
constexpr MOVE_DIR operator*(MOVE_DIR ori, int a) {
	return static_cast<MOVE_DIR>((int)(ori) * a);
}

// Tile Data
enum class TILE_DATA {
	TD_NON = 0,			// 아무것도 아님
	TD_BLOCK,			// 지나갈수 없음
	TD_FLOOR,			// 바닥
	TD_SPIKE,			// 닿으면 죽음

	TD_GOAL = 9
};

// Who can use this step
enum class STEP_FOR { 
	SF_YELLOW = 0,
	SF_PUR, 
	SF_ALL
};