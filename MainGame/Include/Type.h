#pragma once

#include <Windows.h>

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
	TD_NON = 0,			// �ƹ��͵� �ƴ�
	TD_BLOCK,			// �������� ����
	TD_FLOOR,			// �ٴ�
	TD_SPIKE,			// ������ ����

	TD_GOAL = 9
};

// Who can use this step
enum class STEP_FOR { 
	SF_YELLOW = 0,
	SF_PUR, 
	SF_ALL
};