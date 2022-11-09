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

public:
	virtual bool Init() = 0;

	virtual int Update(float fTimeElapsed);
	virtual void Render(HDC hdc);
};
