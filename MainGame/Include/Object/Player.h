#pragma once

#include "MovingObject.h"
#include "../GameManager/GameManager.h"

#define PURPLE_PLAYER 0
#define YELLOW_PLAYER 1


class Player : public MovingObject
{
//private:
//	static int m_iPlayerCnt;

public:
    Player();
    virtual ~Player();
	bool bRender = false;

protected:
    int m_iPlayerNum;

	// for image sprite, walking
	bool m_bMoved = false;
	bool m_bFalling = true;
	bool m_bSit = false;
	bool m_bSitComplete = false;
	bool m_bBeforeSit = false;

	bool m_bWalking = false;

	MOVE_DIR m_eMoveDir = MOVE_DIR::MD_NONE;

	// for image sprite, jumping
	bool m_bLand = false;
	bool m_bBefJump = false;

	float m_fJumpSpeed = 0;

	// 0.5 sec per one image
	float m_fCnt = 0;

	int m_iSpriteY;
	int m_iImageSprite = 4;
	int m_iMaxImageSprite = 4;
	
	// walking sound
	static FMOD_SOUND* m_pWalkSound;
	FMOD_CHANNEL* m_pPlayerChannel;

public:
	int GetPlayerNum() { return m_iPlayerNum; }

	void SetFallingFalse() { m_bFalling = false; m_fJumpSpeed = 0; }
	void SetFallingTrue() { m_bFalling = true; }
	void OnHitCeil() { m_fJumpSpeed = 0; }

public:
	virtual bool Init();

	virtual void Input(float fTimeElapsed, uint8_t& input);
	virtual int Update(float fTimeElapsed);
	virtual void Render(HDC hdc);
};

class PurplePlayer :
	public Player
{
public:
	PurplePlayer() { m_iPlayerNum = PURPLE_PLAYER; }
	virtual ~PurplePlayer() {}

public:
	virtual bool Init();

	virtual void Input(float fTimeElapsed, uint8_t& input);
};


class YellowPlayer :
	public Player
{
public:
	YellowPlayer() { m_iPlayerNum = YELLOW_PLAYER; }
	virtual ~YellowPlayer() {}

public:
	virtual bool Init();

	virtual void Input(float fTimeElapsed, uint8_t& input);
};


