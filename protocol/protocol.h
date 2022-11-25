#pragma once

struct FPOINT {
	float x;
	float y;
};

struct FRECT {
	float left;
	float top;
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

enum class SERVER_PACKET_INFO {
	LOGIN = 0,
	PLAYER_MOVE,
	SCENE_CHANGE
};

enum class CLIENT_PACKET_INFO {
	MOVE = 0,
};

struct PLAYERINFO
{
	int p_id;
	FPOINT p_pos[2];
	unsigned char p_dir;
};

struct S2C_LOGIN_PACKET {
	char type;
	bool b_success;
	char c_id;
};

struct S2C_PLAYER_MOVE_PACKET
{
	char type;
	PLAYERINFO p_data[1];
};

struct S2C_SCENE_CHANGE_PACKET
{
	char type;
	char next_scene_num;
};

struct C2S_MOVE_PACKET
{
	char type;
	char from_c_id;
	unsigned char direction;
};

inline int GetS2CSize(SERVER_PACKET_INFO p)
{
	switch (p) {
	case SERVER_PACKET_INFO::LOGIN:
		return sizeof(S2C_LOGIN_PACKET);

	case SERVER_PACKET_INFO::PLAYER_MOVE:
		return sizeof(S2C_PLAYER_MOVE_PACKET);

	case SERVER_PACKET_INFO::SCENE_CHANGE:
		return sizeof(S2C_SCENE_CHANGE_PACKET);

	default:
		// error
		return -1;
	}
}

inline int GetS2CSize(char p)
{
	return GetS2CSize((SERVER_PACKET_INFO)p);
}

enum PLAYERMOVE {
	KEY_W = 0b10000000,
	KEY_A = 0b01000000,
	KEY_S = 0b00100000,
	KEY_D = 0b00010000,
	KEY_UP = 0b00001000,
	KEY_LEFT = 0b00000100,
	KEY_DOWN = 0b00000010,
	KEY_RIGHT = 0b00000001,
};

