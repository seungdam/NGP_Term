#pragma once
#include "Object.h"

class Button : public Object {
private:
	bool m_bActive = false;
	bool m_bFixed;
	int m_iGroupControll;

public:
	Button() = delete;
	Button(RECT pos, bool bFixed, int group) : m_bFixed{ bFixed }, m_iGroupControll{ group } { SetPosition(pos); }
	virtual ~Button();

public:
	// getter setter
	int getGroupCtrl() const { return m_iGroupControll; }
	bool IsActive() const { return m_bActive; }
	bool IsFixed() const { return m_bFixed; }

	void SetActiveState(bool state) { m_bActive = state; }
public:
	virtual bool Init();

	virtual int Update(float fTimeElapsed);
	virtual void Render(HDC hdc);
};

