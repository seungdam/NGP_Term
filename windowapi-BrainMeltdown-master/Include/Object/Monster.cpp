#include "Monster.h"

CImage Monster::m_tMonsterImg;

Monster::Monster(RECT pos, MOVE_DIR mdr) : m_eMoveDir(mdr) 
{
    SetPosition(pos);
    if (m_tMonsterImg.IsNull()) m_tImg.Load(L"Resource/Monster.bmp");
    if (mdr == MOVE_DIR::MD_NONE) {}
    else {}
}

Monster::~Monster()
{
}

void Monster::Reverse()
{
    m_eMoveDir = m_eMoveDir * -1;
    m_tPosition = m_tBefPos;
}

bool Monster::Init()
{
    SetSpeed(50.0f);
    m_tBefPos = m_tPosition;
    return true;
}

int Monster::Update(float fTimeElapsed)
{
    m_tBefPos = m_tPosition;
    Move((int)(m_eMoveDir) * GetSpeed() * fTimeElapsed, 0);

    m_fCnt += fTimeElapsed;
    if (m_fCnt >= 0.05f) {
        m_fCnt = 0;
        m_iImageSprite++;
        if (m_iImageSprite > m_iMaxImageSprite) m_iImageSprite = 0;
    }

    return 0;
}

void Monster::Render(HDC hdc)
{
    RECT t = (RECT)GetPosition();

    //FrameRect(hdc, &t, (HBRUSH)GetStockObject(WHITE_BRUSH));

    if (m_eMoveDir != MOVE_DIR::MD_NONE)
        m_tImg.TransparentBlt(hdc, t.left, t.top, t.right - t.left, t.bottom - t.top,
            m_iImageSprite * 72, 72 * ((1 - (int)(m_eMoveDir)) >> 1), 70, 70, IMAGE_TRANSPARENT);
    else 
        m_tImg.TransparentBlt(hdc, t.left, t.top, t.right - t.left, t.bottom - t.top,
           0, 72 * 2, 70, 70, IMAGE_TRANSPARENT);
    
}
