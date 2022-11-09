#pragma once
#include "Object.h"
#include "../Type.h"

class Step : public Object {
private:
	int m_iGroup;
	STEP_FOR m_eType;
	bool m_bAlive = true;
	int m_ibuttonAliveCnt = m_bAlive;

private:
	static CImage m_tStepImg;

public:
	Step() = delete;
	Step(STEP_FOR sf, RECT pos) : m_eType(sf) { SetPosition(pos); }
	Step(RECT pos, STEP_FOR sf, bool bAl, int group) : m_eType(sf), m_bAlive(bAl), m_iGroup(group) { SetPosition(pos); }
	virtual ~Step() {}

public:
	STEP_FOR GetType() { return m_eType; }
	bool IsAlive() { return m_bAlive; }

public:
	void SetGroup(int i) { m_iGroup = i; }

	int GetGroup() { return m_iGroup; }
	int GetCount() { return m_ibuttonAliveCnt; }

public:
	void Active() { m_ibuttonAliveCnt++; }
	void DeActive() { m_ibuttonAliveCnt--; if (m_ibuttonAliveCnt < 0) m_ibuttonAliveCnt = 0; }

public:

public:
	virtual bool Init();

	virtual int Update(float fTimeElapsed);
	virtual void Render(HDC hdc);
};

