#pragma once
#pragma warning (disable : 4996)
#pragma comment(lib, "WS2_32.lib")

#include <iostream>
#include <winsock2.h> 
#include <ws2tcpip.h> 
#include <Windows.h>
#include <unordered_map>
#include "../protocol/protocol.h"

#define TILESIZE 40
#define END_SCENE 999

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
	return static_cast<MOVE_DIR>((int)(ori)*a);
}

// Tile Data
enum class TILE_DATA {
	TD_NON = 0,			// ¾Æ¹«°Íµµ ¾Æ´Ô
	TD_BLOCK,			// Áö³ª°¥¼ö ¾øÀ½
	TD_FLOOR,			// ¹Ù´Ú
	TD_SPIKE,			// ´êÀ¸¸é Á×À½

	TD_GOAL = 9
};

// Who can use this step
enum class STEP_FOR {
	SF_YELLOW = 0,
	SF_PUR,
	SF_ALL
};

#define SAFE_DELETE(p)			if(p)	{ delete p; p = NULL; }
#define SAFE_DELETE_ARRY(p)		if(p)	{ delete[] p; p = NULL; }
#define SAFE_RELEASE(p)			if(p)	{ p->Release(); p = NULL; }

template <typename T>
void ReleaseAndCleanVector(T& p) {
	typename T::iterator iter;
	for (iter = p.begin(); iter != p.end(); ++iter) SAFE_DELETE((*iter));

	p.clear();
}

