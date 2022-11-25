#pragma once

#include "../Game.h"

class Object {
protected:
	Object();

public:
	virtual ~Object();

protected:
	FRECT m_tPosition = { 0,0,0,0 };
	CImage m_tImg;

public:
	// getter setter
	FRECT GetPosition() const { return m_tPosition; }

	void SetPosition(RECT const& tPos) { m_tPosition = tPos; }
	void SetPivot(const FPOINT& pivot) {
		FPOINT size = { (m_tPosition.right - m_tPosition.left) / 2.0f, (m_tPosition.bottom - m_tPosition.top) / 2.0f };
		
		m_tPosition.left = pivot.x - size.x;
		m_tPosition.top = pivot.y - size.y;
		m_tPosition.right = pivot.x + size.x;
		m_tPosition.bottom = pivot.y + size.y;
	}

public:
	virtual bool Init() = 0;

	virtual int Update(float fTimeElapsed);
	virtual void Render(HDC hdc);
};
