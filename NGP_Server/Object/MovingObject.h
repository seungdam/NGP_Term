#pragma once
#include "Object.h"

class MovingObject : public Object {
protected:
	MovingObject();
	virtual ~MovingObject();

protected:
	float m_fSpeed = 100.0f;

protected:
	FPOINT m_tBefPos;

public:
	void SetSpeed(float speed) { m_fSpeed = speed; }
	float GetSpeed() const { return m_fSpeed; }

public:
	void Move(float dx, float dy);
	void MoveXFromSpeed(MOVE_DIR eDir);
	void MoveYFromSpeed(MOVE_DIR eDir);

	void GoBack();
	void GoBackX();
	void GoBackY();

public:
	virtual bool Init() = 0;
};