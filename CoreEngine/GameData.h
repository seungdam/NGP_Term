#pragma once
#include <cmath>

#define SERVERPORT 9900
#define SERVERIP "127.0.0.1"

#define MAX_MOVING_STEPS 30

#define TILE_IMAGE_SIZE 70
#define TILE_IMAGE_STRIDE (TILE_IMAGE_SIZE + 2)
#define TILE_IMAGE_FLOOR 4
#define TILE_IMAGE_BLOCK 5
#define IMAGE_TRANSPARENT RGB(80, 40, 0)

#define TILESIZE 40
#define END_SCENE 4
#define LOADING_SCENE 998
#define NEXT_SCENE 997
#define WINDOW_SIZE_WIDTH 1280
#define WINDOW_SIZE_HEIGHT 720


#define TILESIZE 40
#define SERVER_END_SCENE 999
#define CLEAR_STAGE 1

#define MAX_PLAYERS 3
#define EPSILON 0.00001f

struct FPOINT
{
	float x;
	float y;

	bool operator==(const FPOINT& other)
	{
		return
			(abs(x - other.x) < EPSILON) &&
			(abs(y - other.y) < EPSILON);
	}
};

// Move Direction
enum class MOVE_DIR
{
	MD_BACK = -1,
	MD_NONE,
	MD_FRONT,
	MD_UP = -10,
	MD_DOWN = 10
};

constexpr MOVE_DIR operator*(MOVE_DIR original, int a)
{
	return static_cast<MOVE_DIR>((int)(original)*a);
}

// Tile Data
enum class TILE_DATA
{
	TD_NON = 0,			// ¾Æ¹«°Íµµ ¾Æ´Ô
	TD_BLOCK,			// Áö³ª°¥¼ö ¾øÀ½
	TD_FLOOR,			// ¹Ù´Ú
	TD_SPIKE,			// ´êÀ¸¸é Á×À½
	TD_GOAL = 9
};

// Who can use this step
enum class STEP_FOR
{
	SF_YELLOW = 0,
	SF_PUR,
	SF_ALL
};





enum KEY
{
	KEY_W = 0b10000000,
	KEY_A = 0b01000000,
	KEY_S = 0b00100000,
	KEY_D = 0b00010000,
	KEY_UP = 0b00001000,
	KEY_LEFT = 0b00000100,
	KEY_DOWN = 0b00000010,
	KEY_RIGHT = 0b00000001,
};


