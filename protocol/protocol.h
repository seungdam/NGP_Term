#pragma once

struct FPOINT {
	float x;
	float y;
};

struct FRECT {
	float top;
	float left;
	float right;
	float bottom;

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

struct PLAYERINFO
{
	int p_id;
	FPOINT p_pos[2];
	int p_dir;
};

struct S2C_LOGIN_PACKET {
	char type;
	bool b_success;
	char c_id;
};

struct S2C_PLAYER_MOVE_PACKET
{
	char type;
	PLAYERINFO p_data[3];
};

struct S2C_SCENE_CHANGE__PACKET
{
	char type;
	char next_scene_num;
};

struct C2S_MOVE_PACKET
{
	char type;
	char from_c_id;
	char direction;
};
