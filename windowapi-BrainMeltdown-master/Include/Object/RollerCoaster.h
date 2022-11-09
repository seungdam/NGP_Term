#pragma once
#include "MovingObject.h"
class RollerCoaster : public MovingObject {
private:
	RECT m_tInitpos;
	RECT m_tMoveTo;

	int m_iGroup;
	bool m_ibuttonAliveCnt;

	BOOL const m_bAlways;

	STEP_FOR m_eType;
	MOVE_DIR m_eDir;

private:
	static CImage m_tRCImg;

public:
	RollerCoaster() = delete;
	RollerCoaster(RECT pos, STEP_FOR t, int b, int g, RECT mt);
	virtual ~RollerCoaster();

public:
	int GetGroup() { return m_iGroup; }
	STEP_FOR GetType() { return m_eType; }

	void Active() { m_ibuttonAliveCnt = true; }
	void DeActive() { m_ibuttonAliveCnt = false; }

public:
	virtual bool Init();

	virtual int Update(float fTimeElapsed);
	virtual void Render(HDC hdc);
};

