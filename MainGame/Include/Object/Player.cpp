#include "Player.h"
#include "Object.h"
#include "../Core.h"

FMOD_SOUND* Player::m_pWalkSound = NULL;

Player::Player()
{
	RECT t = { 0,0,25,50 };
	m_tPosition = t;
}

Player::~Player()
{
	FMOD_Channel_Stop(m_pPlayerChannel);
}


bool Player::Init()
{
	SetSpeed(250.0f);

	m_iSpriteY = m_eMoveDir == MOVE_DIR::MD_BACK;

	if (m_pWalkSound == NULL) 
		FMOD_System_CreateSound(Core::GetInst().GetFmodSystem(), "Sounds/walkingSound.mp3", FMOD_LOOP_NORMAL, 0, &m_pWalkSound);

	return true;
}

int Player::Update(float fTimeElapsed)
{
	Object::Update(fTimeElapsed);

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
			FMOD_System_PlaySound(Core::GetInst().GetFmodSystem(), m_pWalkSound, NULL, 0, &m_pPlayerChannel);
			FMOD_Channel_SetVolume(m_pPlayerChannel, 0.8f);
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
		FMOD_Channel_Stop(m_pPlayerChannel);
		m_bWalking = false;
	}
	return 0;
}


void Player::Render(HDC hdc)
{
	Object::Render(hdc);
	RECT t = { (LONG)m_tPosition.left, (LONG)m_tPosition.top, (LONG)m_tPosition.right, (LONG)m_tPosition.bottom };

#ifdef DEBUG
	FrameRect(hdc,&t, (HBRUSH)GetStockObject(BLACK_BRUSH));
#endif // DEBUG

	if (bRender)
	m_tImg.TransparentBlt(hdc, t.left - 13, t.top, 50, 50, 
		m_iImageSprite * 72, 72 * m_iSpriteY, 70, 70, IMAGE_TRANSPARENT);


	//printf("%d %d\n", m_iImageSprite, m_iSpriteY);
	//printf("m_eMoveDir : %d\n", m_eMoveDir);
}

/// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

bool PurplePlayer::Init()
{
	m_tImg.Load(L"Resource/player1.png");

	return Player::Init();
}

int PurplePlayer::Update(float fTimeElapsed)
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
	// 이전에 w가 안눌렸고, 지금은 눌린 상태
	if (!(m_befPlayerInput & KEY_UP) && m_PlayerInput & KEY_UP) {
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

	m_befPlayerInput = m_PlayerInput;
	return Player::Update(fTimeElapsed);
}

void PurplePlayer::Input(float fTimeElapsed, uint8_t& input)
{
	m_tBefPos = m_tPosition;

	m_bMoved = false;
	m_bSit = false;

	// Input
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		input |= KEY_LEFT;

		Move(-GetSpeed() * fTimeElapsed / (m_bSitComplete + 1), 0);
		m_eMoveDir = MOVE_DIR::MD_BACK;
		m_bMoved = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		input |= KEY_RIGHT;

		Move(GetSpeed() * fTimeElapsed / (m_bSitComplete + 1), 0);
		m_eMoveDir = MOVE_DIR::MD_FRONT;
		m_bMoved = true;
	}
	if (!m_bFalling) {
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			input |= KEY_UP;

			m_bFalling = true;
			m_fJumpSpeed = -550.0f;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && !m_bLand) {
			input |= KEY_DOWN;

			m_bSit = true;
		}
		else {
			m_bSitComplete = false;
		}
	}

	Player::Update(fTimeElapsed);
}

bool YellowPlayer::Init()
{
	m_tImg.Load(L"Resource/player2.bmp");
	
	return Player::Init();
}

int YellowPlayer::Update(float fTimeElapsed)
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
	// 이전에 w가 안눌렸고, 지금은 눌린 상태
	if (!(m_befPlayerInput & KEY_W) && m_PlayerInput & KEY_W) {
		if (m_PlayerInput & KEY_W) {
			m_bFalling = true;
			m_fJumpSpeed = -550.0f;
		}
		if (m_PlayerInput & KEY_S && !m_bLand) {
			m_bSit = true;
		}
		else m_bSitComplete = false;
	}

	m_befPlayerInput = m_PlayerInput;
	return Player::Update(fTimeElapsed);
}

void YellowPlayer::Input(float fTimeElapsed, uint8_t& input)
{
	m_tBefPos = m_tPosition;

	m_bMoved = false;
	m_bSit = false;

	if (GetAsyncKeyState('A') & 0x8000) {
		input |= KEY_A;

		Move(-GetSpeed() * fTimeElapsed / (m_bSitComplete + 1), 0);
		m_eMoveDir = MOVE_DIR::MD_BACK;
		m_bMoved = true;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		input |= KEY_D;

		Move(GetSpeed() * fTimeElapsed / (m_bSitComplete + 1), 0);
		m_eMoveDir = MOVE_DIR::MD_FRONT;
		m_bMoved = true;
	}
	if (!m_bFalling) {
		if (GetAsyncKeyState('W') & 0x8000) {
			input |= KEY_W;

			m_bFalling = true;
			m_fJumpSpeed = -550.0f;
		}
		if (GetAsyncKeyState('S') & 0x8000 && !m_bLand) {
			input |= KEY_S;

			m_bSit = true;
		}
		else m_bSitComplete = false;
	}

	Player::Update(fTimeElapsed);
}
