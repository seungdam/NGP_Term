#include "../Game.h"
#include "Player.h"

Player::Player()
{
	m_tSize = {12.5f, 25.0f};
}

Player::~Player()
{
}


bool Player::Init()
{
	SetSpeed(250.0f);

	m_iSpriteY = m_eMoveDir == MOVE_DIR::MD_BACK;

	return true;
}

void Player::Input(float fTimeElapsed)
{
	// setSprite
	m_iSpriteY = m_eMoveDir == MOVE_DIR::MD_BACK;

	// go down and set sprite
	if (m_bFalling) {
		Move(0.0f, m_fJumpSpeed * fTimeElapsed);

		m_fJumpSpeed += 500 * fTimeElapsed * 3;
		if (m_fJumpSpeed >= 500) m_fJumpSpeed = 500;
		m_bBefJump = true;
	}

	// begin sit and set sprite;
	if (m_bFalling || m_bLand) {
		m_iSpriteY += 2;
	}
	else if (m_bSit) {
		m_iSpriteY += 4;
		if (m_bSitComplete) m_iSpriteY += 2;
	}
	if (m_bBefJump && !m_bFalling) {
		m_fCnt = 0;
		m_iImageSprite = 9;
		m_bLand = true;
		m_bBefJump = false;
	}

	if (m_bSit && !m_bSitComplete) {
		m_iImageSprite *= m_bBeforeSit;
		m_bBeforeSit = true;
		m_fCnt += fTimeElapsed;
		if (m_fCnt >= 0.025f) {
			m_iImageSprite++;
			m_fCnt = 0;
		}

		if (m_iImageSprite > 9 - m_iPlayerNum) {
			m_bSitComplete = true;
			m_iImageSprite = 5 - m_iPlayerNum;
			m_iSpriteY += 2;
		}
	}
	else if (m_bFalling) {
		// 공중에서 점프중
		if (m_iImageSprite < 0) m_iImageSprite = (int)(m_fJumpSpeed / 300) + 3;
		else m_iImageSprite = (int)(m_fJumpSpeed / 150) + 3;

		m_bBefJump = true;
	}
	else if (m_bLand) {
		// 착지
		m_fCnt += fTimeElapsed;
		if (m_fCnt >= (0.025f + 0.025f * m_iPlayerNum)) {
			m_iImageSprite++;
			m_fCnt = 0;
		}
		
		if (m_iImageSprite > 15 - (3 * m_iPlayerNum)) {
			m_bLand = false;
			m_iSpriteY -= 2;
			m_iImageSprite = 4;
		}
	}
	else if (m_bMoved) {
		// 단순 좌우 이동
		m_fCnt += fTimeElapsed;

		if (m_fCnt >= 0.075f) {
			m_iImageSprite++;
			m_fCnt = 0;
		}
		m_iImageSprite %= m_iMaxImageSprite + m_bSitComplete - m_iPlayerNum;
		
		m_bBeforeSit = false;
		if (!m_bWalking) {
			m_bWalking = true;
		}

	}
	else {
		// idle
		m_iImageSprite = m_iMaxImageSprite + m_bSitComplete - (m_iPlayerNum * m_bSitComplete);
		m_fCnt = 0;

		m_bBeforeSit = false;
	}


	if (!m_bMoved || m_bFalling) {
		m_bWalking = false;
	}
	
}

int Player::Update(float fTimeElapsed)
{
	Input(fTimeElapsed);
	Object::Update(fTimeElapsed);
	return 0;
}


/// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

bool PurplePlayer::Init()
{
	return Player::Init();
}

void PurplePlayer::Input(float fTimeElapsed)
{
	m_tBefPos = m_tPosition;

	m_bMoved = false;
	m_bSit = false;

	// Input
	if (m_PlayerInput & KEY_LEFT) {
		Move(-GetSpeed() * fTimeElapsed / (m_bSitComplete + 1), 0);
		m_eMoveDir = MOVE_DIR::MD_BACK;
		m_bMoved = true;
	}
	if (m_PlayerInput & KEY_RIGHT) {
		Move(GetSpeed() * fTimeElapsed / (m_bSitComplete + 1), 0);
		m_eMoveDir = MOVE_DIR::MD_FRONT;
		m_bMoved = true;
	}
	if (!m_bFalling) {
		if (m_PlayerInput & KEY_UP) {
			m_bFalling = true;
			m_fJumpSpeed = -550.0f;
		}
		if (m_PlayerInput & KEY_DOWN && !m_bLand) {
			m_bSit = true;
		}
		else {
			m_bSitComplete = false;
		}
	}

	Player::Input(fTimeElapsed);
}

bool YellowPlayer::Init()
{
	return Player::Init();
}

void YellowPlayer::Input(float fTimeElapsed)
{
	m_tBefPos = m_tPosition;

	m_bMoved = false;
	m_bSit = false;

	if (m_PlayerInput & KEY_A) {
		Move(-GetSpeed() * fTimeElapsed / (m_bSitComplete + 1), 0);
		m_eMoveDir = MOVE_DIR::MD_BACK;
		m_bMoved = true;
	}
	if (m_PlayerInput & KEY_D) {
		Move(GetSpeed() * fTimeElapsed / (m_bSitComplete + 1), 0);
		m_eMoveDir = MOVE_DIR::MD_FRONT;
		m_bMoved = true;
	}
	if (!m_bFalling) {
		if (m_PlayerInput & KEY_W) {
			m_bFalling = true;
			m_fJumpSpeed = -550.0f;
		}
		if (m_PlayerInput & KEY_S&& !m_bLand) {
			m_bSit = true;
		}
		else m_bSitComplete = false;
	}

	Player::Input(fTimeElapsed);
}
