#include "../Game.h"
#include "RollerCoaster.h"


RollerCoaster::RollerCoaster(RECT pos, STEP_FOR t, int b, int g, RECT mt) 
	: m_tInitpos(pos), m_eType(t), m_ibuttonAliveCnt(0), m_iGroup(g), m_tMoveTo(mt), m_bAlways(b)
{ 
	FPOINT size{ (pos.right - pos.left) / 2.0f, (pos.bottom - pos.top) / 2.0f };
	FPOINT pivot{ pos.left + size.x, pos.top + size.y };

	SetPosition(pivot);
	SetSize(size);

	if (m_tInitpos.left != m_tMoveTo.left) m_eDir = m_tInitpos.left > m_tMoveTo.left ? MOVE_DIR::MD_BACK : MOVE_DIR::MD_FRONT;
	else m_eDir = m_tInitpos.top > m_tMoveTo.top ? MOVE_DIR::MD_UP : MOVE_DIR::MD_DOWN;
}

RollerCoaster::~RollerCoaster()
{
}

bool RollerCoaster::Init()
{

	return true;
}

int RollerCoaster::Update(float fTimeElapsed)
{
	//printf("%d\n", m_ibuttonAliveCnt);
	if (!(m_ibuttonAliveCnt + m_bAlways)) return 1;
	// move deltatime * speed
	if (-5 < (int)(m_eDir) && (int)(m_eDir) < 5) Move(fTimeElapsed * GetSpeed() * (int)(m_eDir), 0);
	else Move(0, fTimeElapsed * GetSpeed() * (int)(m_eDir) / 10);
	// if reached pos? speed * -1

	FRECT temp = GetPosition();

	int goal;
	switch (m_eDir) {
	case MOVE_DIR::MD_BACK:
		goal = m_tMoveTo.left;

		if (goal > temp.left) {
			m_eDir = m_eDir * -1;

			temp = m_tMoveTo;
			m_tMoveTo = m_tInitpos;
			m_tInitpos = temp;
		}
		break;

	case MOVE_DIR::MD_FRONT:
		goal = m_tMoveTo.right;

		if (goal < temp.right) {
			m_eDir = m_eDir * -1;

			temp = m_tMoveTo;
			m_tMoveTo = m_tInitpos;
			m_tInitpos = temp;
		}
		break;

	case MOVE_DIR::MD_UP:
		goal = m_tMoveTo.top;

		if (goal > temp.top) {
			m_eDir = m_eDir * -1;

			temp = m_tMoveTo;
			m_tMoveTo = m_tInitpos;
			m_tInitpos = temp;
		}
		break;

	case MOVE_DIR::MD_DOWN:
		goal = m_tMoveTo.bottom;

		if (goal < temp.bottom) {
			m_eDir = m_eDir * -1;

			temp = m_tMoveTo;
			m_tMoveTo = m_tInitpos;
			m_tInitpos = temp;
		}
		break;

	}

	return 0;
}
