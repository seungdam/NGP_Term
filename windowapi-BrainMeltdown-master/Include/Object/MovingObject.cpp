#include "MovingObject.h"

MovingObject::MovingObject()
{
}

MovingObject::~MovingObject()
{
}

void MovingObject::Move(float dx, float dy)
{
	m_tPosition.OffsetRect(dx, dy);
}

void MovingObject::MoveXFromSpeed(MOVE_DIR eDir)
{
	m_tPosition.OffsetRect(m_fSpeed * (int)(eDir), 0.0f);
}

void MovingObject::MoveYFromSpeed(MOVE_DIR eDir)
{
	m_tPosition.OffsetRect(0.0f, m_fSpeed * (int)(eDir));
}

void MovingObject::GoBack()
{
	m_tPosition = m_tBefPos;

	m_tBefPos = m_tPosition;
}

void MovingObject::GoBackX()
{
	m_tPosition.left = m_tBefPos.left;
	m_tPosition.right = m_tBefPos.right;

	m_tBefPos = m_tPosition;
}

void MovingObject::GoBackY()
{
	m_tPosition.top = m_tBefPos.top;
	m_tPosition.bottom = m_tBefPos.bottom;

	m_tBefPos = m_tPosition;
}

