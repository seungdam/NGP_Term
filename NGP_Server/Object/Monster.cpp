#include "../Game.h"
#include "Monster.h"


Monster::Monster(RECT pos, MOVE_DIR mdr) : m_eMoveDir(mdr) 
{
    FPOINT size{ (pos.right - pos.left) / 2.0f, (pos.bottom - pos.top) / 2.0f };
    FPOINT pivot{ pos.left + size.x, pos.top + size.y };

    SetPosition(pivot);
    SetSize(size);
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

