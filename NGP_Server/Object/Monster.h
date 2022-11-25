#pragma once
#include "MovingObject.h"


class Monster : public MovingObject {
private:
	MOVE_DIR m_eMoveDir = MOVE_DIR::MD_NONE;

public:
	Monster() = delete;
	Monster(RECT pos, MOVE_DIR mdr);
	virtual ~Monster();

public:
	void Reverse();

	MOVE_DIR GetDirection() const { return m_eMoveDir; }

private:
	int m_iImageSprite = 0;
	int m_iMaxImageSprite = 7;
	float m_fCnt = 0;

public:
	virtual bool Init();

	virtual int Update(float fTimeElapsed);
};
