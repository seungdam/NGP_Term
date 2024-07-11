#pragma once
#include "Object.h"

class Step : public Object {
private:
	int m_iGroup;
	STEP_FOR m_eType;
	bool m_bAlive = true;
	int m_ibuttonAliveCnt = m_bAlive;

private:

public:
	Step() = delete;
	Step(STEP_FOR sf, RECT pos) : m_eType(sf) {
		FPOINT size{ (pos.right - pos.left) / 2.0f, (pos.bottom - pos.top) / 2.0f };
		FPOINT pivot{ pos.left + size.x, pos.top + size.y };

		SetPosition(pivot);
		SetSize(size);
	}
	Step(RECT pos, STEP_FOR sf, bool bAl, int group) : m_eType(sf), m_bAlive(bAl), m_iGroup(group) {
		FPOINT size{ (pos.right - pos.left) / 2.0f, (pos.bottom - pos.top) / 2.0f };
		FPOINT pivot{ pos.left + size.x, pos.top + size.y };

		SetPosition(pivot);
		SetSize(size);
	}
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
};

