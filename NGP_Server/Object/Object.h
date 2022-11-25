#pragma once

class Object {
protected:
	Object();

public:
	virtual ~Object();

protected:
	//FRECT m_tPosition = { 0,0,0,0 };
	FPOINT m_tPosition{ 0.0f, 0.0f };		// �Ǻ��� �簢�� �߾ӿ� ��ġ���ִ�.
	FPOINT m_tSize;							// �ʱ�ȭ ���־�� �Ѵ�

public:
	// getter setter
	FRECT GetPosition() const { 
		FRECT temp;
		temp.left = m_tPosition.x - m_tSize.x;
		temp.top = m_tPosition.y - m_tSize.y;
		temp.right = m_tPosition.x + m_tSize.x;
		temp.bottom = m_tPosition.y + m_tSize.y;
		return temp;
	}

	FPOINT GetPivot() const { return m_tPosition; }

	void SetPosition(FPOINT const& tPos) { m_tPosition = tPos; }
	void SetSize(FPOINT const& size) { m_tSize = size; }

public:
	virtual bool Init() = 0;

	virtual int Update(float fTimeElapsed);
};
