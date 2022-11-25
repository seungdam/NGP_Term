#include "../Game.h"
#include "MovingObject.h"

MovingObject::MovingObject()
{
}

MovingObject::~MovingObject()
{
}

void MovingObject::Move(float dx, float dy)
{
	m_tPosition.x += dx;
	m_tPosition.y += dy;
}

void MovingObject::MoveXFromSpeed(MOVE_DIR eDir)
{
	m_tPosition.x += m_fSpeed * (int)(eDir);
}

void MovingObject::MoveYFromSpeed(MOVE_DIR eDir)
{
	m_tPosition.y += m_fSpeed * (int)(eDir);
}

void MovingObject::GoBack()
{
	m_tPosition = m_tBefPos;

	m_tBefPos = m_tPosition;
}

void MovingObject::GoBackX()
{
	m_tPosition.x = m_tBefPos.x;
	m_tBefPos = m_tPosition;
}

void MovingObject::GoBackY()
{
	m_tPosition.y = m_tBefPos.y;
	m_tBefPos = m_tPosition;
}

